#pragma once
#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <DirectXMath.h>

#define ELEMENT_TYPES \
		X(Float) \
		X(Float2) \
		X(Float3) \
		X(Float4) \
		X(Matrix) \
		X(Bool)


enum ElemType
{
#define X(Element) Element,
		ELEMENT_TYPES
#undef X

		Struct,
		Array,
		Empty,
};


namespace DynamicCB
{
	//= Attributes for each type ==============

	template<ElemType type>
	struct ElementType
	{
		static constexpr bool Valid_ = false;

		//using CPUType// type used in the CPU side
		//static constexpr size_t HLSLSize // size of type on GPU side
		//static constexpr const char* code // code used for generating signature of layout
		//static constexpr bool valid  // metaprogramming flag to check validity of Map <param>
	};
	template<> struct ElementType<Float>
	{
		using CPUType = float;
		static constexpr size_t HLSLSize_ = sizeof(CPUType);
		static constexpr const char* ElementCode_ = "F1";
		static constexpr bool Valid_ = true;
	};
	template<> struct ElementType<Float2>
	{
		using CPUType = DirectX::XMFLOAT2;
		static constexpr size_t HLSLSize_ = sizeof(CPUType);
		static constexpr const char* ElementCode_ = "F2";
		static constexpr bool Valid_ = true;
	};
	template<> struct ElementType<Float3>
	{
		using CPUType = DirectX::XMFLOAT3;
		static constexpr size_t HLSLSize_ = sizeof(CPUType);
		static constexpr const char* ElementCode_ = "F3";
		static constexpr bool Valid_ = true;
	};
	template<> struct ElementType<Float4>
	{
		using CPUType = DirectX::XMFLOAT4;
		static constexpr size_t HLSLSize_ = sizeof(CPUType);
		static constexpr const char* ElementCode_ = "F4";
		static constexpr bool Valid_ = true;
	};
	template<> struct ElementType<Matrix>
	{
		using CPUType = DirectX::XMFLOAT4X4;
		static constexpr size_t HLSLSize_ = sizeof(CPUType);
		static constexpr const char* ElementCode_ = "M4";
		static constexpr bool Valid_ = true;
	};
	template<> struct ElementType<Bool>
	{
		using CPUType = bool;
		static constexpr size_t HLSLSize_ = 4u;
		static constexpr const char* ElementCode_ = "BL";
		static constexpr bool Valid_ = true;
	};

	//Check that we have the definitions for all element types
#define X(Element) static_assert(ElementType<Element>::Valid_, "Missing attribute definition for " #Element);
	ELEMENT_TYPES
#undef X

		//= Reverse lookups from CPUType to ElementType ===========

		template<typename T>
	struct ReverseLookup
	{
		static constexpr bool Valid_ = false;
	};

#define X(Element) \
		template<> struct ReverseLookup<typename ElementType<Element>::CPUType> \
		{ \
			static constexpr ElemType Type_ = Element; \
			static constexpr bool Valid_ = true; \
		};
	ELEMENT_TYPES;
#undef X

	/* Used as part of a tree to describe the layout of the data buffer. */
	class LayoutElement
	{
	private:
		struct ExtraDataBase
		{
			virtual ~ExtraDataBase() = default;
		};

		friend class RawLayout;
		friend struct ExtraData;
	public:

		/* Recursively generates a string returning a unique identifier for this layout */
		std::string GetSignature() const;
		bool Exists() const;
		std::pair<size_t, const LayoutElement*> CalculateIndexingOffset(size_t Offset, size_t Index) const;

		// = Operators ===

		LayoutElement& operator[] (const std::string& Key);
		const LayoutElement& operator[](const std::string& Key) const;


		LayoutElement& T();
		const LayoutElement& T() const;

		// Offset based functions only work after finalization
		size_t GetOffsetBegin() const;
		size_t GetOffsetEnd() const;
		size_t GetSizeInBytes() const;

		//= Struct adding

		LayoutElement& Add(ElemType AddedType, std::string Name);
		template<ElemType Added>
		LayoutElement& Add(std::string Key)
		{
			return Add(Added, std::move(Key));
		}

		//= Array Adding
		// Sets the type and the number of elements
		LayoutElement& Set(ElemType AddedType, size_t Size);

		template<ElemType TypeAdded>
		LayoutElement& Set(size_t size)
		{
			return Set(TypeAdded, size);
		}

		/* Returns the offset of the type for read/write */
		template<typename T>
		size_t Resolve() const
		{
			switch (Type_)
			{
#define X(Element) case Element: assert(typeid(ElementType<Element>::CPUType) == typeid(T)); return *Offset_;
				ELEMENT_TYPES
#undef X

			default:
				assert("Tried to resolve unsupported element" && false);
				return 0u;
			}
		}

	private:
		LayoutElement() = default;
		LayoutElement(ElemType TypeIn);

		std::string GetSignatureForStruct() const;
		std::string GetSignatureForArray() const;

		/* Sets all offsets, adds padding if needed */
		size_t Finalize(size_t OffsetIn);
		size_t Finalize_Struct(size_t OffsetIn);
		size_t Finalize_Array(size_t OffsetIn);

		/* Returns singleton empty layout */
		static LayoutElement& GetEmptyElement()
		{
			static LayoutElement Empty{};
			return Empty;
		}

		/* Returns the value of the offset moved to the next 16-byte boundary if not on one */
		static size_t AdvanceToBoundary(size_t Offset);
		/* Returns true if the memory block crosses a boundary */
		static bool CrossesBoundary(size_t Offset, size_t Size);
		/* Advance to the next offset if the memory block crosses a boundary */
		static size_t AdvanceIfCrossesBoundary(size_t Offset, size_t Size);
		/* Checks the validity of the string for use as a struct key */
		static bool ValidateSymbolName(const std::string& Name);

	private:
		std::optional<size_t> Offset_;
		ElemType Type_ = Empty;
		std::unique_ptr<ExtraDataBase> ExtraData_;
	};

	class Layout
	{
		friend class LayoutCodex;
		friend class Buffer;
	public:
		size_t GetSizeInBytes() const;
		std::string GetSignature() const;
	protected:
		Layout(std::shared_ptr<LayoutElement> Root);
		std::shared_ptr<LayoutElement> Root_;
	};

	class RawLayout : public Layout
	{
		friend class LayoutCodex;
	public:
		RawLayout();

		template<ElemType type>
		LayoutElement& Add(const std::string& Key)
		{
			return Root_->Add<type>(Key);
		}

		LayoutElement& operator[](const std::string& Key);
	private:
		void ClearRoot();
		/* Finalizes the layout, then yields the root element*/
		std::shared_ptr<LayoutElement> DeliverRoot();
	};

	/* Represents a comple registered layout shell. Cannot change the tree */
	class CookedLayout : public Layout
	{
		friend class LayoutCodex;
		friend class Buffer;
	public:
		const LayoutElement& operator[](const std::string& Key) const;
		std::shared_ptr<LayoutElement> ShareRoot() const;
	private:
		CookedLayout(std::shared_ptr<LayoutElement> Root);
		std::shared_ptr<LayoutElement> RelinquishRoot() const;
	};

	/* Proxy type used when keying into the buffer, read only*/
	class ConstElementRef
	{
		friend class Buffer;
		friend class ElementRef;
	public:

		class Pointer
		{
			friend ConstElementRef;
		public:

			/* Returns read-only pointer to supported type */
			template<typename T>
			operator const T* () const
			{
				static_assert(ReverseLookup<std::remove_const_t<T>>::Valid_, "Unsupported type used in pointer conversion");
				return &static_cast<const T&>(*Ref_);
			}
		private:
			Pointer(const ConstElementRef* Reference);
			const ConstElementRef* Ref_;
		};

	public:

		bool Exists() const;
		ConstElementRef operator[](const std::string& Key) const; //Index in for the current element as a struct
		ConstElementRef operator[](size_t Index) const;//Index in for the current element as an array
		ConstElementRef::Pointer operator&() const;

		/* Conversion to read as a supported type */
		template<typename T>
		operator const T& () const
		{
			static_assert(ReverseLookup<std::remove_const_t<T>>::Valid_, "Unsupported type used in conversion");
			return *reinterpret_cast<const T*>(Bytes_ + Offset_ + Layout_->Resolve<T>());
		}

	private:
		/* Should only be constructable by other references or the buffer */
		ConstElementRef(const LayoutElement* Layout, const char* Bytes, size_t Offset);

		size_t Offset_;
		const LayoutElement* Layout_;
		const char* Bytes_;
	};

	/* Similar to above class, allows writing */
	class ElementRef
	{
		friend class Buffer;
	public:

		class Pointer
		{
			friend ElementRef;
		public:

			template<typename T>
			operator T* () const
			{
				static_assert(ReverseLookup<std::remove_const_t<T>>::valid, "Unsupported type used in pointer conversion");
				return &static_cast<T&>(*Ref_);
			}

		private:
			Pointer(ElementRef* Ref);
			ElementRef* Ref_;
		};

	public:
		operator ConstElementRef() const;
		ElementRef operator[](const std::string Key) const;
		ElementRef operator[](size_t Index) const;
		Pointer operator&() const;

		/* Conversion for read/write */
		template<typename T>
		operator T& () const
		{
			static_assert(ReverseLookup <std::remove_const_t<T>>::Valid_, "Unsupported Type Used in Assignment");
			return *reinterpret_cast<T*>(Bytes_ + Offset_ + Layout_->Resolve<T>());
		}

		/* Conversion for write */
		template<typename T>
		T& operator=(const T& RHS) const
		{
			static_assert(ReverseLookup <std::remove_const_t<T>>::Valid_, "Unsupported Type Used in Assignment");
			return static_cast<T&>(*this) = RHS;
		}

		template<typename T>
		bool SetIfExists(const T& Val)
		{
			if (!Exists())
				return false;

			*this = Val;
			return true;
		}

		bool Exists() const;

	private:
		ElementRef(const LayoutElement* Layout, char* Bytes, size_t Offset);
		size_t Offset_;
		const LayoutElement* Layout_;
		char* Bytes_;



	};

	class Buffer
	{
	public:
		Buffer(RawLayout&& Layout);
		Buffer(const CookedLayout* Layout);
		Buffer(CookedLayout&& Layout);
		Buffer(const Buffer&);
		Buffer(Buffer&&);

		ElementRef operator[] (const std::string& Key);
		ConstElementRef operator[] (const std::string& Key) const;

		const char* GetData() const;
		size_t GetSizeInBytes() const;
		const LayoutElement& GetRootLayoutElement() const;

		/* Copys from another buffer, the layouts must match. */
		void CopyFrom(const Buffer&);

		std::shared_ptr<LayoutElement> ShareLayoutRoot() const;

	private:
		std::shared_ptr<LayoutElement> LayoutRoot_;
		std::vector<char> Bytes_;
	};
}