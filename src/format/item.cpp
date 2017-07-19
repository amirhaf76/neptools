#include "item.hpp"
#include "context.hpp"
#include "raw_item.hpp"
#include "../utils.hpp"
#include <algorithm>
#include <iostream>

#if !defined(NEPTOOLS_WITHOUT_LUA) && !defined(NEPTOOLS_BINDING_GENERATOR)
#include "format/builder.lua.h"
#endif

#define NEPTOOLS_LOG_NAME "item"
#include "../logger_helper.hpp"

namespace Neptools
{

Item::~Item()
{
    Item::Dispose();
}

void Item::Inspect_(std::ostream& os, unsigned indent) const
{
    for (const auto& it : GetLabels())
    {
        os << "label(" << Quoted(it.GetName());
        if (it.GetPtr().offset != 0)
            os << ", " << it.GetPtr().offset;
        os << ")\n";
    }
    Indent(os, indent);
}

void Item::UpdatePosition(FilePosition npos)
{
    position = npos;
    Fixup();
}

void Item::Replace_(const NotNull<SmartPtr<Item>>& nitem)
{
    auto ctx = GetContext();
    // move labels
    nitem->labels.swap(labels); // intrusive move op= does this... (but undocumented)
    for (auto& l : nitem->labels)
        l.ptr.item = nitem.get();

    // update pointermap
    nitem->position = position;
    auto it = ctx->pmap.find(position);
    if (it != ctx->pmap.end() && it->second == this)
        it->second = nitem.get();

    auto& list = GetParent()->GetChildren();
    auto self = Iterator();

    list.insert(self, *nitem);
    list.erase(self);
}

void Item::Slice(SliceSeq seq)
{
    NEPTOOLS_ASSERT(std::is_sorted(seq.begin(), seq.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; }));

    SmartPtr<Item> do_not_delete_this_until_returning{this};

    LabelsContainer lbls{std::move(labels)};
    auto& list = GetParent()->GetChildren();
    auto it = Iterator();
    it = list.erase(it);

    auto ctx = GetContext();
    auto& pmap = ctx->pmap;
    auto empty = pmap.empty();
    // remove this from pmap
    if (!empty)
    {
        auto iter = pmap.find(position);
        if (iter != pmap.end() && iter->second == this)
            pmap.erase(iter);
    }

    FilePosition offset = 0;
    auto base_pos = position;

    auto label = lbls.unlink_leftmost_without_rebalance();
    for (auto& el : seq)
    {
        NEPTOOLS_ASSERT(el.first->labels.empty());
        while (label && label->ptr.offset < el.second)
        {
            label->ptr.item = el.first.get();
            label->ptr.offset -= offset;
            el.first->labels.insert(*label);

            label = lbls.unlink_leftmost_without_rebalance();
        }

        // move in place
        el.first->position = base_pos + offset;
        list.insert(it, *el.first);

        // add to pmap if needed
        if (!empty)
            // may throw! but only used during parsing, and an exception there
            // is fatal, so it's not really a problem
            pmap.emplace(el.first->position, &*el.first);

        offset = el.second;
    }
    NEPTOOLS_ASSERT(label == nullptr);
}

void Item::Dispose() noexcept
{
    NEPTOOLS_ASSERT(labels.empty() && !GetParent());
    if (auto ctx = GetContextMaybe())
    {
        auto it = ctx->pmap.find(position);
        if (it != ctx->pmap.end() && it->second == this)
        {
            WARN << "Item " << this << " unlinked from pmap in Dispose" << std::endl;
            ctx->pmap.erase(it);
        }
    }

    context.reset();
}

std::ostream& operator<<(std::ostream& os, const Item& item)
{
    item.Inspect(os);
    return os;
}

void ItemWithChildren::Dump_(Sink& sink) const
{
    for (auto& c : GetChildren())
        c.Dump(sink);
}

void ItemWithChildren::InspectChildren(std::ostream& os, unsigned indent) const
{
    if (GetChildren().empty()) return;

    os << ":build(function()\n";
    for (auto& c : GetChildren())
    {
        c.Inspect_(os, indent+1);
        os << '\n';
    }
    Indent(os, indent) << "end)";
}


FilePosition ItemWithChildren::GetSize() const
{
    FilePosition ret = 0;
    for (auto& c : GetChildren())
        ret += c.GetSize();
    return ret;
}

void ItemWithChildren::Fixup_(FilePosition offset)
{
    FilePosition pos = position + offset;
    for (auto& c : GetChildren())
    {
        c.UpdatePosition(pos);
        pos += c.GetSize();
    }
}

void ItemWithChildren::MoveNextToChild(size_t size) noexcept
{
    auto& list = GetParent()->GetChildren();
    // make sure we have a ref when erasing...
    SmartPtr<Item> nchild = &asserted_cast<RawItem&>(
        *++Iterator()).Split(0, size);
    list.erase(nchild->Iterator());
    GetChildren().push_back(*nchild);
}

void ItemWithChildren::Dispose() noexcept
{
    GetChildren().clear();
    Item::Dispose();
}

}

#include "../container/parent_list.lua.hpp"
#include "../lua/table_ret_wrap.hpp"
#include "../lua/owned_shared_ptr_wrap.hpp"
NEPTOOLS_PARENT_LIST_LUAGEN(item, false, Neptools::Item, Neptools::ItemListTraits);
#include "item.binding.hpp"
