#include "WinPlatform.h"

using namespace std;

/////////////////////////////////////////////////////////////
//get info about target file from a .lnk file
//Function adapted from 
//https://docs.microsoft.com/en-us/windows/desktop/shell/links
Lnk_Info get_LnkInfo(const fs::path& LnkFile_Path) {
	Lnk_Info info;
	//Initialize a COM library
	if(FAILED(CoInitialize(NULL))) {
		cout << "get_Lnkpath(const nstring&): CoInitialize(NULL) failed!\n" << endl;
		return Lnk_Info();
	}
	IShellLink* shell_link_instance = nullptr;
	IPersistFile* file_interface = nullptr;
	//create a IShellLink instance
	if(FAILED(CoCreateInstance(CLSID_ShellLink,
		//create a single object
		NULL,
		//default
		CLSCTX_INPROC_SERVER,
		IID_IShellLink,
		(LPVOID*)&shell_link_instance))) {
		cout << "get_Lnkpath(const nstring&): Failed to create IShellLink instance!\n" << endl;
		goto failed;
	}
	//associate a file interface instance with the IShellLink instance
	if(FAILED(shell_link_instance->QueryInterface(IID_IPersistFile, (void**)&file_interface))) {
		cout << "get_Lnkpath(const nstring&): Failed to associate file interface with IShellLink instance!\n" << endl;
		goto failed;
	}
	//open the file in file interface
	if(FAILED(file_interface->Load(LnkFile_Path.c_str(), STGM_READ))) {
		cout << "get_Lnkpath(const nstring&): File interface failed to load specified file!\n" << endl;
		goto failed;
	}
	//resolve the link
	if(FAILED(shell_link_instance->Resolve(NULL, SLR_UPDATE))) {
		cout << "get_Lnkpath(const nstring&): IShellLink failed to resolve link!\n" << endl;
		return Lnk_Info();
	}
	//get the path and attributes of target
	nchar temp_path[MAX_PATH];
	if(FAILED(shell_link_instance->GetPath(temp_path, MAX_PATH, &(info.target_attributes), NULL))) {
		cout << "get_Lnkpath(const nstring&): IShellLink failed to get path!\n" << endl;
		return Lnk_Info();
	}
	info.target_path = temp_path;
	//get the working directory
	if(FAILED(shell_link_instance->GetWorkingDirectory(temp_path,MAX_PATH))) {
		cout << "get_Lnkpath(const nstring&): IShellLink failed to get working directory!\n" << endl;
		return Lnk_Info();
	}
	info.cur_dir = temp_path;
	//get the icon path and index if mutiple exist in file
	if(FAILED(shell_link_instance->GetIconLocation(temp_path,MAX_PATH,&info.icon_index))) {
		cout << "get_Lnkpath(const nstring&): IShellLink failed to get Icon Location!\n" << endl;
		return Lnk_Info();
	}
	info.icon_path = temp_path;
	//get the show command
	if(FAILED(shell_link_instance->GetShowCmd(&info.show_cmd))) {
		cout << "get_Lnkpath(const nstring&): IShellLink failed to get Icon Location!\n" << endl;
		return Lnk_Info();
	}
	//release all the resources
	file_interface->Release();
	shell_link_instance->Release();
	CoUninitialize();
	return info;
failed:
	if(file_interface)file_interface->Release();
	if(shell_link_instance)shell_link_instance->Release();
	CoUninitialize();
	return Lnk_Info();
}
