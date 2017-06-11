// Auto generated code, do not edit. See gen_binding in project root.
#include "lua/user_type.hpp"

namespace Neptools::Lua
{

// class neptools.item
template<>
void TypeRegister::DoRegister<::Neptools::Item>(TypeBuilder& bld)
{
    bld.Inherit<::Neptools::Item, ::Neptools::Dumpable>();

    bld.AddFunction<
        static_cast<RefCountedPtr<::Neptools::Context> (::Neptools::Item::*)() noexcept>(&::Neptools::Item::GetContextMaybe)
    >("get_context_maybe");
    bld.AddFunction<
        static_cast<::Neptools::NotNull<RefCountedPtr<::Neptools::Context> > (::Neptools::Item::*)() noexcept>(&::Neptools::Item::GetContext)
    >("get_context");
    bld.AddFunction<
        static_cast<::Neptools::ItemWithChildren * (::Neptools::Item::*)() noexcept>(&::Neptools::Item::GetParent)
    >("get_parent");
    bld.AddFunction<
        static_cast<::Neptools::FilePosition (::Neptools::Item::*)() const noexcept>(&::Neptools::Item::GetPosition)
    >("get_position");
    bld.AddFunction<
        static_cast<void (::Neptools::Item::*)(const ::Neptools::NotNull<RefCountedPtr<::Neptools::Item> > &) noexcept>(&::Neptools::Item::Replace<Check::Throw>)
    >("replace");
    bld.AddFunction<
        TableRetWrap<static_cast<const ::Neptools::Item::LabelsContainer & (::Neptools::Item::*)() const>(&::Neptools::Item::GetLabels)>::Wrap
    >("get_labels");

}
static TypeRegister::StateRegister<::Neptools::Item> reg_neptools_item;

}


const char ::Neptools::Item::TYPE_NAME[] = "neptools.item";

namespace Neptools::Lua
{

// class neptools.item_with_children
template<>
void TypeRegister::DoRegister<::Neptools::ItemWithChildren>(TypeBuilder& bld)
{
    bld.Inherit<::Neptools::ItemWithChildren, ::Neptools::Item>();

    bld.AddFunction<
        OwnedSharedPtrWrap<static_cast<::Neptools::ItemList & (::Neptools::ItemWithChildren::*)() noexcept>(&::Neptools::ItemWithChildren::GetChildren)>::Wrap
    >("get_children");

}
static TypeRegister::StateRegister<::Neptools::ItemWithChildren> reg_neptools_item_with_children;

}


const char ::Neptools::ItemWithChildren::TYPE_NAME[] = "neptools.item_with_children";
