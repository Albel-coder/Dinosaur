#include "Download.h"

string Download::GetVersion()
{
    DownloadProgress progress;

    DeleteUrlCacheEntry(L"https://github.com/Albel-coder/Dinosaur/blob/main/resources/version/LastVersion.txt");
    HRESULT hr = URLDownloadToFile(0, L"https://github.com/Albel-coder/Dinosaur/blob/main/resources/version/LastVersion.txt", L"Tabs.txt", 0, static_cast<IBindStatusCallback*>(&progress));

    ifstream in("Tabs.txt");
    if (in.is_open())
    {
        //cout << "\nCreate file";
        string path = "__________"; int i = 0;
        char ch = ' ';
        while (!in.eof())
        {
            in >> ch;
            if (ch == '_')
            {
                i++;
            }
            else
            {
                i = 0;
            }
            if (i == 10)
            {
                i = 0;
                //cout << "\nFind text!\n\n";
                while (i != 10 || !in.eof())
                {
                    in >> ch;
                    if (ch == '_')
                    {
                        i++;
                        if (i == 10)
                        {
                            //cout << "\n\nEnd output\n\n";
                            break;
                        }
                    }
                    else
                    {
                        i = 0;
                        Version.push_back(ch);
                    }
                }
                break;
            }
        }
    }
    in.close();
    if (Version == "")
    {
        InternetConnection = false;
    }

    CheckUpdate = true;
    remove("Tabs.txt");
    return Version;
}

bool Download::IsVersionNew()
{
    if (currentVersion == Version)
    {
        return true;
    }
    else
    {
        return false;
    }
}
