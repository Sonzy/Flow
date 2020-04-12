#include "Flowpch.h"
#include "LayoutCodex.h"

Flow::DCB::CookedLayout Flow::DCB::LayoutCodex::Resolve(DCB::RawLayout&& Layout)
{
	auto Signature = Layout.GetSignature();
	auto& Map = Get().Map_;
	const auto i = Map.find(Signature);
	// idential layout already exists
	if (i != Map.end())
	{
		// input layout is expected to be cleared after Resolve
		// so just throw away the layout tree
		Layout.ClearRoot();
		return { i->second };
	}
	// otherwise add layout root element to map
	auto result = Map.insert({ std::move(Signature),Layout.DeliverRoot() });
	// return layout with additional reference to root
	return { result.first->second };
}

Flow::DCB::LayoutCodex& Flow::DCB::LayoutCodex::Get()
{
	static Flow::DCB::LayoutCodex Codex;
	return Codex;
}
