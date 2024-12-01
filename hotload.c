b8 LibNeedReload(LibCodeInfo *lib)
{
    FileAttribute file_stat = {0};
    b8 ret = GetFileInfo(lib->dll_name, &file_stat);
    if (ret)
    {
        if (file_stat.last_write != lib->last_write)
        {
            b8 lock_file_existed = lib->lock_file.str ? GetFileInfo(lib->lock_file, &file_stat) : 0;
            return !lock_file_existed;
        }
    }
    
    return 0;
}

b8 LibHotLoad(LibCodeInfo *lib)
{
    b8 result = 0;
    FileAttribute file_stat = {0};
    
    
    if (GetFileInfo(lib->dll_name, &file_stat))
    {
        if (CopyFileB(lib->dll_name_temp, lib->dll_name, TRUE))
        {
            
            lib->hdll = (u8*)LoadLibraryA((LPCSTR)lib->dll_name_temp.str);
            if(lib->hdll)
            {
                lib->func_ptr = (u8*)GetProcAddress((HMODULE)lib->hdll, (LPCSTR)lib->func_name.str);
                result = (lib->func_ptr != 0);
                if (result) 
                {
                    lib->last_write = file_stat.last_write;
                }
            }
            else
            {
                LogInfo("Failed to load dll\n");
            }
        }
        else 
        {
            LogInfo("Failed to copy file\n");
        }
    }
    
    return result;
}

void LibUnload(LibCodeInfo *lib)
{    
    FreeLibrary((HMODULE)lib->hdll);
    lib->hdll = 0;
}
