
rem  ------------------¿½±´common------------------
XCOPY ..\source\common\*.h ..\include\common\ /s  /e /y
XCOPY ..\source\common\*.hpp ..\include\common\ /s  /e /y
XCOPY ..\source\common\*.inl ..\include\common\ /s  /e /y


rem  ------------------¿½±´webglPlus------------------
XCOPY ..\source\webglPlus\*.h ..\include\webglPlus\ /s  /e /y
XCOPY ..\source\webglPlus\*.hpp ..\include\webglPlus\ /s  /e /y
XCOPY ..\source\webglPlus\*.inl ..\include\webglPlus\ /s  /e /y


rem  ------------------¿½±´render------------------
XCOPY ..\source\render\*.h ..\include\render\ /s  /e /y
XCOPY ..\source\render\*.hpp ..\include\render\ /s  /e /y

rem  ------------------¿½±´render------------------
XCOPY ..\source\conch\Bridge\*.h ..\include\conch\Bridge\ /s  /e /y


rem ------------------¿½±´conchRuntime------------------
XCOPY ..\build\conchRuntime\proj.ios\conchRuntime\conchRuntime.h ..\include\conchRuntime\ /s  /y
XCOPY ..\build\conchRuntime\proj.ios\conchRuntime\conchConfig.h ..\include\conchRuntime\ /s  /y
XCOPY ..\build\conchRuntime\proj.ios\conchRuntime\PlatformInterface\*.h ..\include\conchRuntime\PlatformInterface\ /s  /e /y

rem ------------------¿½±´Íê±Ï------------------