#include <stdio.h>
#include "importer.h"

#include <Python.h>

#define PRINTF printf

static uint64_t now_us()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

static uint64_t now_ms()
{
    return now_us() / 1000;
}

static int import_archive(const char *archive, const char *pwd)
{

    char code[4096];
    size_t cap = sizeof(code);

    PyObject *main_module = PyImport_AddModule("__main__");
    PyObject *main_dict = PyModule_GetDict(main_module);

    PyRun_String(IMPORTER, Py_file_input, main_dict, main_dict);

    snprintf(code, cap, "ZipImporterWrapper('%s', b'%s').load()", archive, pwd);
    PyRun_String(code, Py_file_input, main_dict, main_dict);

    snprintf(code, cap, "sys.path.append('/home/atlas/Projects/Python/archive/.venv/lib/python3.8/site-packages/libs')");
    PyRun_String(code, Py_file_input, main_dict, main_dict);

}

static int demo2()
{
    const char *PWD = "atlas";
    const char *ZIP = "/home/atlas/Projects/Python/archive/archive.zip";
    const char *PDF = "/home/atlas/Downloads/test/bug/3-字体变黑黑框.pdf";
    const char *DOCX = "/home/atlas/Projects/C/libpython38/output.docx";

    char code[4096];
    size_t cap = sizeof(code);

    Py_Initialize();

    uint64_t beg = now_ms();

    PRINTF("%s start\n", __func__);

    if(import_archive(ZIP, PWD))
    {
        return -1;
    }

    snprintf(code, cap, "import pdf2docx");
    printf("running: %s\n", code);
    PyRun_String(code, Py_file_input, main_dict, main_dict);

    snprintf(code, cap, "pdf2docx.Converter('%s').convert('%s')", PDF, DOCX);
    printf("running: %s\n", code);
    PyRun_String(code, Py_file_input, main_dict, main_dict);

    uint64_t end = now_ms();
    PRINTF("%s elapsed %" PRIu64 "ms\n", __func__, end - beg);

    Py_Finalize();
    return 0;
}

int demo()
{
    // 初始化 Python 解释器
    Py_Initialize();

    PyObject *main_module = PyImport_AddModule("__main__"); // 获取主模块
    PyObject *main_dict = PyModule_GetDict(main_module);    // 获取模块字典

    // 多行 Python 代码作为字符串
    const char *code =
        "def greet(name):\n"
        "    return f'Hello, {name}!'\n"
        "result = greet('world')\n"
        "print(result)\n";

    PyObject *globals = PyDict_New();
    PyObject *locales = PyDict_New();

    // 执行代码
    // PyRun_String(code, Py_file_input, globals, locales);
    PyRun_String(code, Py_file_input, main_dict, main_dict);

    // 取回执行结果
    PyObject *result = PyDict_GetItemString(main_dict, "result"); // 获取变量 `result`
    // PyObject *result = PyDict_GetItemString(globals, "result"); // 获取变量 `result`

    // 输出结果
    if (result)
    {
        PyObject *result_str = PyObject_Str(result);            // 将结果转为字符串
        const char *result_cstr = PyUnicode_AsUTF8(result_str); // 转换为 C 字符串
        printf("Result: %s\n", result_cstr);
        Py_XDECREF(result_str);
    }
    else
    {
        printf("Hasnot Result\n");
    }

    // 清理 Python 解释器
    Py_Finalize();

    return 0;
}

int main()
{
    demo2();

    return 0;
}
