#pragma once

/**
 * @namespace stdx::os::win32
 * @brief Common Controls (commctrl.h) wrappers.
 */
export namespace stdx::os::win32 {
    #if defined(_WIN32) && __has_include(<commctrl.h>)
    // Handles
    using ImageListHandle = ::HIMAGELIST;
    using PropertySheetPageHandle = ::HPROPSHEETPAGE;
    using TreeItemHandle = ::HTREEITEM;

    // Init
    using InitCommonControlsEx = ::INITCOMMONCONTROLSEX;

    // Color scheme
    using ColorScheme = ::COLORSCHEME;

    // List / tree / tab items
    using ListViewItem = ::LVITEMA;
    using ListViewItemW = ::LVITEMW;
    using ListViewColumn = ::LVCOLUMNA;
    using ListViewColumnW = ::LVCOLUMNW;
    using TreeViewItem = ::TVITEMA;
    using TreeViewItemW = ::TVITEMW;
    using TreeViewItemEx = ::TVITEMEXA;
    using TreeViewItemExW = ::TVITEMEXW;
    using TreeViewInsertStruct = ::TVINSERTSTRUCTA;
    using TreeViewInsertStructW = ::TVINSERTSTRUCTW;
    using TabControlItem = ::TCITEMA;
    using TabControlItemW = ::TCITEMW;

    // Notifications
    using NotifyMessageHeader = ::NMHDR;
    using NotifyMessageHeaderPointer = ::LPNMHDR;
    using NotifyMessageMouse = ::NMMOUSE;
    using NotifyMessageKey = ::NMKEY;
    using NotifyMessageChar = ::NMCHAR;
    using NotifyMessageCustomDraw = ::NMCUSTOMDRAW;
    using NotifyMessageListViewCustomDraw = ::NMLVCUSTOMDRAW;
    using NotifyMessageTreeViewCustomDraw = ::NMTVCUSTOMDRAW;

    // Image list operations
    using ::ImageList_Create;
    using ::ImageList_Destroy;
    using ::ImageList_Add;
    using ::ImageList_AddMasked;
    using ::ImageList_Remove;
    using ::ImageList_ReplaceIcon;
    using ::ImageList_GetIcon;
    using ::ImageList_GetImageCount;
    using ::ImageList_Draw;
    using ::ImageList_DrawEx;
    using ::ImageList_SetImageCount;
    using ::ImageList_GetIconSize;

    // Init / version
    using ::InitCommonControls;
    using ::InitCommonControlsEx;

    // Misc helpers
    using ::CreateStatusWindowA;
    using ::CreateStatusWindowW;
    using ::CreateToolbarEx;
    using ::CreatePropertySheetPageA;
    using ::CreatePropertySheetPageW;
    using ::PropertySheetA;
    using ::PropertySheetW;
    #endif
}
