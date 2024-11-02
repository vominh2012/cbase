/* date = October 27th 2024 8:52 am */

#ifndef HOTLOAD_H
#define HOTLOAD_H

typedef struct  LibCodeInfo {
    CString dll_name;
    CString dll_name_temp;
    CString lock_file; // optional
    CString func_name;
    void *hdll;
    u64 last_write;
    void *func_ptr;
} LibCodeInfo;

b8 LibNeedReload(LibCodeInfo *lib);
b8 LibHotLoad(LibCodeInfo *lib);
void LibUnload(LibCodeInfo *lib);

#endif //HOTLOAD_H
