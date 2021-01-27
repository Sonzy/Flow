//= Includes ===========================================

#include "Flowpch.h"
#include "DynamicConstantBuffer.h"
#include "Flow\Rendering\Core\Bindables\LayoutCodex.h"

//= Class Definitions ==================================

namespace DynamicCB
{
	struct ExtraData //TODO: Elaborate on this
	{
		struct Struct : public LayoutElement::ExtraDataBase
		{
			std::vector<std::pair<std::string, LayoutElement>> LayoutElements_;
		};
		struct Array : public LayoutElement::ExtraDataBase
		{
			std::optional<LayoutElement> LayoutElement_;
			size_t Size_;
		};
	};



	std::string LayoutElement::GetSignature() const
	{
		switch (Type_)
		{
#define X(Element) case Element: return ElementType<Element>::ElementCode_;
			ELEMENT_TYPES
#undef X

		case Struct:
			return GetSignatureForStruct();
		case Array:
			return GetSignatureForArray();
		default:
			assert("Bad Type in signature generation" && false);
			return "BAD_SIGNATURE_TYPE";
		}
	}

	bool LayoutElement::Exists() const
	{
		return Type_ != Empty;
	}

	std::pair<size_t, const LayoutElement*> LayoutElement::CalculateIndexingOffset(size_t Offset, size_t Index) const
	{
		assert("Indexing into non-array" && Type_ == Array);

		const auto& Data = static_cast<ExtraData::Array&>(*ExtraData_);
		assert(Index < Data.Size_);

		return { Offset + Data.LayoutElement_->GetSizeInBytes() * Index, &*Data.LayoutElement_ };
	}

	LayoutElement& LayoutElement::operator[](const std::string& Key)
	{
		assert("Keying into non-struct" && Type_ == Struct);

		for (auto& Element : static_cast<ExtraData::Struct&>(*ExtraData_).LayoutElements_)
		{
			if (Element.first == Key)
				return Element.second;
		}

		return GetEmptyElement();
	}

	const LayoutElement& LayoutElement::operator[](const std::string& Key) const
	{
		return const_cast<LayoutElement&>(*this)[Key];
	}

	LayoutElement& LayoutElement::T()
	{
		assert("Accessing T of a non-array" && Type_ == Array);
		return *static_cast<ExtraData::Array&>(*ExtraData_).LayoutElement_;
	}

	const LayoutElement& LayoutElement::T() const
	{
		return const_cast<LayoutElement&>(*this).T();
	}

	size_t LayoutElement::GetOffsetBegin() const
	{
		return *Offset_;
	}

	size_t LayoutElement::GetOffsetEnd() const
	{
		switch (Type_)
		{
#define X(Element) case Element: return *Offset_ + ElementType<Element>::HLSLSize_;
			ELEMENT_TYPES
#undef X

		case Struct:
			{
				const auto& Data = static_cast<ExtraData::Struct&>(*ExtraData_);
				return AdvanceToBoundary(Data.LayoutElements_.back().second.GetOffsetEnd());
			}
		case Array:
		{
			const auto& Data = static_cast<ExtraData::Array&>(*ExtraData_);
			return *Offset_ + AdvanceToBoundary(Data.LayoutElement_->GetSizeInBytes()) * Data.Size_;
		}
		default:
			assert("Tried to get offset of bad element type" && false);
			return 0u;
		}
	}

	size_t LayoutElement::GetSizeInBytes() const
	{
		return GetOffsetEnd() - GetOffsetBegin();
	}

	LayoutElement& LayoutElement::Add(ElemType AddedType, std::string Name)
	{
		assert("Adding to a non-struct element" && Type_ == Struct);
		assert("Invalid symbol name in struct" && ValidateSymbolName(Name));

		//Ensure we aren't adding somethign that already exists
		auto& StructData = static_cast<ExtraData::Struct&>(*ExtraData_);
		for (auto& Element : StructData.LayoutElements_)
		{
			if (Element.first == Name)
				assert("Adding duplicate name to struct" && false);
		}

		StructData.LayoutElements_.emplace_back(std::move(Name), LayoutElement{ AddedType });
		return *this;
	}

	LayoutElement& LayoutElement::Set(ElemType AddedType, size_t Size)
	{
		assert("Tried to call Set() on non-array type" && Type_ == Array);
		assert(Size != 0u);

		auto& ArrayData = static_cast<ExtraData::Array&>(*ExtraData_);
		ArrayData.LayoutElement_ = { AddedType };
		ArrayData.Size_ = Size;
		return *this;
	}

	LayoutElement::LayoutElement(ElemType TypeIn)
		: Type_(TypeIn)
	{
		assert(TypeIn != Empty);

		if (TypeIn == Struct)
			ExtraData_ = std::unique_ptr<ExtraData::Struct>{ new ExtraData::Struct() };
		else if (TypeIn == Array)
			ExtraData_ = std::unique_ptr<ExtraData::Array>{ new ExtraData::Array() };
	}

	std::string LayoutElement::GetSignatureForStruct() const
	{
		using namespace std::string_literals;

		std::string Signature = "Struct{"s;
		for (const auto& Element : static_cast<ExtraData::Struct&>(*ExtraData_).LayoutElements_)
		{
			Signature += Element.first + ":"s + Element.second.GetSignature() + ";"s;
		}
		Signature + "}"s; //TODO: probs wanna fix this
		return Signature;
	}

	std::string LayoutElement::GetSignatureForArray() const
	{
		using namespace std::string_literals;

		const auto& Data = static_cast<ExtraData::Array&>(*ExtraData_);
		return "Array:"s + std::to_string(Data.Size_) + "{"s + Data.LayoutElement_->GetSignature() + "}"s;
	}

	size_t LayoutElement::Finalize(size_t OffsetIn)
	{
		switch (Type_)
		{
#define X(Element) case Element: Offset_ = AdvanceIfCrossesBoundary(OffsetIn, ElementType<Element>::HLSLSize_); \
									return *Offset_ + ElementType<Element>::HLSLSize_;
			ELEMENT_TYPES
#undef X

		case Struct:
			return Finalize_Struct(OffsetIn);
		case Array:
			return Finalize_Array(OffsetIn);
		default:
			assert("Bad type used in Finalize()" && false);
			return 0u;
		}
	}

	size_t LayoutElement::Finalize_Struct(size_t OffsetIn)
	{
		auto& Data = static_cast<ExtraData::Struct&>(*ExtraData_);
		assert(Data.LayoutElements_.size() != 0);

		Offset_ = AdvanceToBoundary(OffsetIn);
		auto OffsetNext = *Offset_;

		for (auto& Element : Data.LayoutElements_)
		{
			OffsetNext = Element.second.Finalize(OffsetNext);
		}
		return OffsetNext;
	}

	size_t LayoutElement::Finalize_Array(size_t OffsetIn)
	{
		auto& Data = static_cast<ExtraData::Array&>(*ExtraData_);
		assert(Data.Size_ != 0u);

		Offset_ = AdvanceToBoundary(OffsetIn);
		Data.LayoutElement_->Finalize(*Offset_);

		return GetOffsetEnd();
	}

	size_t LayoutElement::AdvanceToBoundary(size_t Offset)
	{
		return Offset + (16u - Offset % 16u) % 16u;
	}

	bool LayoutElement::CrossesBoundary(size_t Offset, size_t Size)
	{
		const auto End = Offset + Size;
		const auto PageStart = Offset / 16u;
		const auto PageEnd = End / 16u;
		return (PageStart != PageEnd && End % 16 != 0u) || Size > 16u;
	}

	size_t LayoutElement::AdvanceIfCrossesBoundary(size_t Offset, size_t Size)
	{
		return CrossesBoundary(Offset, Size) ? AdvanceToBoundary(Offset) : Offset;
	}

	bool LayoutElement::ValidateSymbolName(const std::string& Name)
	{
		return //Allows alphanumeric + underscores, can't start with a number
			!Name.empty() &&
			!std::isdigit(Name.front()) &&
			std::all_of(Name.begin(), Name.end(), [](char c) {	return std::isalnum(c) || c == '_';	});
	}


	Layout::Layout(std::shared_ptr<LayoutElement> Root)
		: Root_{ std::move(Root) }
	{
	}

	size_t Layout::GetSizeInBytes() const
	{
		return Root_->GetSizeInBytes();
	}

	std::string Layout::GetSignature() const
	{
		return Root_->GetSignature();
	}



	RawLayout::RawLayout()
		: Layout{ std::shared_ptr<LayoutElement>{new LayoutElement(Struct)} }
	{
	}

	LayoutElement& RawLayout::operator[](const std::string& Key)
	{
		return (*Root_)[Key];
	}

	void RawLayout::ClearRoot()
	{
		*this = RawLayout();
	}

	std::shared_ptr<LayoutElement> RawLayout::DeliverRoot()
	{
		auto Temp = std::move(Root_);
		Temp->Finalize(0);
		*this = RawLayout();
		return std::move(Temp);
	}



	CookedLayout::CookedLayout(std::shared_ptr<LayoutElement> Root)
		: Layout(std::move(Root))
	{
	}

	const LayoutElement& CookedLayout::operator[](const std::string& Key) const
	{
		return (*Root_)[Key];
	}

	std::shared_ptr<LayoutElement> CookedLayout::ShareRoot() const
	{
		return Root_;
	}

	std::shared_ptr<LayoutElement> CookedLayout::RelinquishRoot() const
	{
		return std::move(Root_);
	}



	ConstElementRef::ConstElementRef(const LayoutElement* Layout, const char* Bytes, size_t Offset)
		: Offset_(Offset), Layout_(Layout), Bytes_(Bytes)
	{
	}

	ConstElementRef ConstElementRef::operator[](const std::string& Key) const
	{
		return { &(*Layout_)[Key], Bytes_, Offset_ };
	}

	ConstElementRef ConstElementRef::operator[](size_t Index) const
	{
		const auto IndexingData = Layout_->CalculateIndexingOffset(Offset_, Index);
		return { IndexingData.second, Bytes_, IndexingData.first };
	}

	ConstElementRef::Pointer ConstElementRef::operator&() const
	{
		return Pointer{ this };
	}

	bool ConstElementRef::Exists() const
	{
		return Layout_->Exists();
	}

	ConstElementRef::Pointer::Pointer(const ConstElementRef* Reference)
		: Ref_(Reference)
	{
	}




	ElementRef::ElementRef(const LayoutElement* Layout, char* Bytes, size_t Offset)
		: Offset_(Offset), Layout_(Layout), Bytes_(Bytes)
	{
	}

	ElementRef::operator ConstElementRef() const
	{
		return { Layout_, Bytes_, Offset_ };
	}

	ElementRef ElementRef::operator[](const std::string Key) const
	{
		return { &(*Layout_)[Key], Bytes_, Offset_ };
	}

	ElementRef ElementRef::operator[](size_t Index) const
	{
		const auto IndexingData = Layout_->CalculateIndexingOffset(Offset_, Index);
		return { IndexingData.second, Bytes_, IndexingData.first };
	}

	ElementRef::Pointer ElementRef::operator&() const
	{
		return Pointer{ const_cast<ElementRef*>(this) };
	}

	bool ElementRef::Exists() const
	{
		return Layout_->Exists();
	}

	ElementRef::Pointer::Pointer(ElementRef* Ref)
		: Ref_(Ref)
	{
	}




	Buffer::Buffer(RawLayout&& Layout)
		: Buffer(LayoutCodex::Resolve(std::move(Layout)))
	{
	}

	Buffer::Buffer(const CookedLayout& Layout)
		: LayoutRoot_(Layout.ShareRoot()), Bytes_(LayoutRoot_->GetOffsetEnd())
	{
	}

	Buffer::Buffer(CookedLayout&& Layout)
		: LayoutRoot_(Layout.RelinquishRoot()), Bytes_(LayoutRoot_->GetOffsetEnd())
	{
	}

	Buffer::Buffer(const Buffer& Buf)
		: LayoutRoot_(Buf.LayoutRoot_), Bytes_(Buf.Bytes_)
	{
	}

	Buffer::Buffer(Buffer&& Buf)
		: LayoutRoot_(std::move(Buf.LayoutRoot_)), Bytes_(std::move(Buf.Bytes_))
	{
	}

	ElementRef Buffer::operator[](const std::string& Key)
	{
		return { &(*LayoutRoot_)[Key],Bytes_.data(),0u };
	}

	ConstElementRef Buffer::operator[](const std::string& Key) const
	{
		return const_cast<Buffer&>(*this)[Key];
	}

	const char* Buffer::GetData() const
	{
		return Bytes_.data();
	}

	size_t Buffer::GetSizeInBytes() const
	{
		return Bytes_.size();
	}

	const LayoutElement& Buffer::GetRootLayoutElement() const
	{
		return *LayoutRoot_;
	}

	void Buffer::CopyFrom(const Buffer& Other)
	{
		assert(&GetRootLayoutElement() == &Other.GetRootLayoutElement());
		std::copy(Other.Bytes_.begin(), Other.Bytes_.end(), Bytes_.begin());
	}

	std::shared_ptr<LayoutElement> Buffer::ShareLayoutRoot() const
	{
		return LayoutRoot_;
	}
}