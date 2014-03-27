#include <UnitTest++.h>
#include <tchar.h>

extern TCHAR *normalize_path(TCHAR *path);

int path_ret(const TCHAR *realpath, const TCHAR *path)
{
	TCHAR *ret;
	TCHAR *auto_path;
	int ret_result;

	auto_path = _tcsdup(path);
	ret = normalize_path(auto_path);

	if(realpath == NULL){
		ret_result = (ret == NULL ? 0 : 1);
	}else{
		ret_result =  _tcscmp(ret, realpath);
	}
	free(auto_path);

	return ret_result;
}

TEST(Normalize_Path)
{
	CHECK_EQUAL(path_ret(_T("a:\\a\\b\\c"), _T("a:\\a\\b\\c\\")), 0);
	CHECK_EQUAL(path_ret(_T("A:\\a\\b\\c"), _T("A:\\a\\b\\c\\")), 0);
	CHECK_EQUAL(path_ret(_T("z:\\a\\b\\c"), _T("z:\\a\\b\\c\\")), 0);
	CHECK_EQUAL(path_ret(_T("Z:\\a\\b\\c"), _T("Z:\\a\\b\\c\\")), 0);

	CHECK_EQUAL(path_ret(_T("a:\\a\\b\\c"), _T("a:/a/b/c/")), 0);
	CHECK_EQUAL(path_ret(_T("A:\\a\\b\\c"), _T("A:/a/b/c/")), 0);
	CHECK_EQUAL(path_ret(_T("z:\\a\\b\\c"), _T("z:/a/b/c/")), 0);
	CHECK_EQUAL(path_ret(_T("Z:\\a\\b\\c"), _T("Z:/a/b/c/")), 0);

	CHECK_EQUAL(path_ret(NULL, _T("a:a\\b\\c\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("A:a\\b\\c\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("z:a\\b\\c\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("Z:a\\b\\c\\")), 0);

	CHECK_EQUAL(path_ret(NULL, _T("1:a\\b\\c\\")), 0);

	CHECK_EQUAL(path_ret(_T("a:\\路径1\\路径2\\路径3"),
		_T("a:\\路径1\\路径2\\路径3\\")), 0);

	CHECK_EQUAL(path_ret(_T("磁盘符:\\路径1\\路径2\\路径3"),
		_T("磁盘符:\\路径1\\路径2\\路径3\\")), 0);

	/*
	CHECK_EQUAL(path_ret(_T("符:\\路径1\\路径2\\路径3"),
		_T("符:\\路径1\\路径2\\路径3\\")), 0);
*/
	CHECK_EQUAL(path_ret(_T("a:\\"), _T("a:\\a\\b\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("A:\\"), _T("A:\\a\\b\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("z:\\"), _T("z:\\a\\b\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("Z:\\"), _T("Z:\\a\\b\\..\\..\\..")), 0);

	CHECK_EQUAL(path_ret(_T("a:\\"), _T("a:\\a/b\\../..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("A:\\"), _T("A:/a\\b\\../..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("z:\\"), _T("z:\\a/b\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("Z:\\"), _T("Z:\\a\\b\\../..\\..")), 0);

	CHECK_EQUAL(path_ret(_T("a:\\aaa"), _T("a:\\c:\\..\\aaa")), 0);


	CHECK_EQUAL(path_ret(_T(""), _T(".")), 0);
	CHECK_EQUAL(path_ret(_T(" "), _T(" ")), 0);
	CHECK_EQUAL(path_ret(_T("x"), _T("x")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\d"), _T("\\a\\b\\c\\d")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\d"), _T("\\a\\b\\c\\d\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b"), _T("\\a\\\\b")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b"), _T("\\\\a\\\\b\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\.\\.\\.\\.\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a"), _T("\\a\\b\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\a\\b\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\\\a\\\\b\\..\\\\\\\\..\\..\\\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\x"), _T("\\\\a\\\\..\\x\\\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("a\\b\\..\\..\\..\\c")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("..")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("..\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("a\\..\\..")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("a\\..\\..\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("a\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("a\\..\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\..\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\..\\..\\..\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("..\\this")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("..\\this\\is\\ok")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("..\\this\\..\\ok")), 0);
	CHECK_EQUAL(path_ret(NULL, _T("x\\..\\..\\b")), 0);
	CHECK_EQUAL(path_ret(_T("b"), _T("x\\..\\b")), 0);
	CHECK_EQUAL(path_ret(_T("b"), _T(".\\b")), 0);
	CHECK_EQUAL(path_ret(_T("b"), _T(".\\b\\.")), 0);
	CHECK_EQUAL(path_ret(_T("b"), _T(".\\.\\b\\.\\.")), 0);
	CHECK_EQUAL(path_ret(_T("b"), _T(".\\.\\b")), 0);
	CHECK_EQUAL(path_ret(_T("a\\b"), _T(".\\a\\b")), 0);
	CHECK_EQUAL(path_ret(_T(".b"), _T(".b")), 0);
	CHECK_EQUAL(path_ret(_T(".b"), _T(".\\.b")), 0);
	CHECK_EQUAL(path_ret(_T(".metadata\\b"), _T(".metadata\\b")), 0);
	CHECK_EQUAL(path_ret(_T(".metadata\\b"), _T(".\\.metadata\\b")), 0);
	CHECK_EQUAL(path_ret(_T("..a"), _T("..a")), 0);
	CHECK_EQUAL(path_ret(_T("\\..a"), _T("\\..a")), 0);
	CHECK_EQUAL(path_ret(_T("\\..a\\..b"), _T("\\..a\\..b")), 0);
	CHECK_EQUAL(path_ret(_T("\\.....a\\..b"), _T("\\.....a\\..b")), 0);
	CHECK_EQUAL(path_ret(_T("....\\...."), _T("....\\....")), 0);
	CHECK_EQUAL(path_ret(_T("..a..\\..b.."), _T("..a..\\..b..")), 0);
	CHECK_EQUAL(path_ret(_T("\\"), _T("\\.")), 0);
	CHECK_EQUAL(path_ret(_T("\\c"), _T("\\..\\c")), 0);
	CHECK_EQUAL(path_ret(_T("\\a"), _T("\\.\\a")), 0);
	CHECK_EQUAL(path_ret(_T("\\.a"), _T("\\.a\\a\\..")), 0);
	CHECK_EQUAL(path_ret(NULL, _T(".\\..\\..\\")), 0);
	CHECK_EQUAL(path_ret(NULL, _T(".\\..\\.\\..\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\.\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\g\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g?y"), _T("\\a\\b\\c\\g?y")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g#s"), _T("\\a\\b\\c\\g#s")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g?y#s"), _T("\\a\\b\\c\\g?y#s")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\;x"), _T("\\a\\b\\c\\;x")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g;x"), _T("\\a\\b\\c\\g;x")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g;x?y#s"), _T("\\a\\b\\c\\g;x?y#s")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c"), _T("\\a\\b\\c\\.")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c"), _T("\\a\\b\\c\\.\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b"), _T("\\a\\b\\c\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b"), _T("\\a\\b\\c\\..\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\g"), _T("\\a\\b\\c\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a"), _T("\\a\\b\\c\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\a"), _T("\\a\\b\\c\\..\\..\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\g"), _T("\\a\\b\\c\\..\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g."), _T("\\a\\b\\c\\g.")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\.g"), _T("\\a\\b\\c\\.g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g.."), _T("\\a\\b\\c\\g..")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\..g"), _T("\\a\\b\\c\\..g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\g"), _T("\\a\\b\\c\\.\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\.\\g\\.")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g\\h"), _T("\\a\\b\\c\\g\\.\\h")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\h"), _T("\\a\\b\\c\\g\\..\\h")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g;x=1\\y"), _T("\\a\\b\\c\\g;x=1\\.\\y")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\y"), _T("\\a\\b\\c\\gx=1\\..\\y")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\.\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\g\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g?y"), _T("\\a\\b\\c\\g?y")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g#s"), _T("\\a\\b\\c\\g#s")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g?y#s"), _T("\\a\\b\\c\\g?y#s")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\;x;"), _T("\\a\\b\\c\\;x;")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g;x;"), _T("\\a\\b\\c\\g;x;")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g;x?y#s"), _T("\\a\\b\\c\\g;x?y#s")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c"), _T("\\a\\b\\c\\.")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c"), _T("\\a\\b\\c\\.\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b"), _T("\\a\\b\\c\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b"), _T("\\a\\b\\c\\..\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\g"), _T("\\a\\b\\c\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a"), _T("\\a\\b\\c\\..\\..")), 0);
	CHECK_EQUAL(path_ret(_T("\\a"), _T("\\a\\b\\c\\..\\..\\")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\g"), _T("\\a\\b\\c\\..\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\g"), _T("\\a\\b\\c\\..\\..\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\g"), _T("\\a\\b\\c\\..\\..\\..\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g."),_T("\\a\\b\\c\\g.")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\.g"), _T("\\a\\b\\c\\.g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g.."), _T("\\a\\b\\c\\g..")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\..g"), _T("\\a\\b\\c\\..g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\g"), _T("\\a\\b\\c\\.\\..\\g")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g"), _T("\\a\\b\\c\\.\\g\\.")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g\\h"), _T("\\a\\b\\c\\g\\.\\h")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\h"), _T("\\a\\b\\c\\g\\..\\h")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\g;x=1\\y"), _T("\\a\\b\\c\\g;x=1\\.\\y")), 0);
	CHECK_EQUAL(path_ret(_T("\\a\\b\\c\\y"), _T("\\a\\b\\c\\gx=1\\..\\y")), 0);

	CHECK_EQUAL(path_ret(_T("\\aaaaaa\\bbbb"), 
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\aaaaaa\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\bbbb\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")
		_T("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\")), 0);	
	CHECK_EQUAL(path_ret(_T("\\aaaaaa\\bbbb"), 
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("////////////////////////////aaaaaa//////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("////////////////////////////bbbb//////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")
		_T("//////////////////////////////////////////////////////////////////")), 0);	

}

TEST(Normalize_Path2)
{
	CHECK_EQUAL(path_ret(_T("!:\\路径1\\路径2\\路径3"),
		_T("!:\\路径1\\路径2\\路径3\\")), 0);
}
