call ndk-build clean
call ndk-build TARGET_LINK=0 NDK_DEBUG=0 CONFIGURATION=StaticRelease
call ndk-build TARGET_LINK=0 NDK_DEBUG=1 CONFIGURATION=StaticDebug
rmdir /s /q libs
rmdir /s /q obj
call ndk-build clean
call ndk-build TARGET_LINK=1 NDK_DEBUG=0 CONFIGURATION=DynamicRelease
call ndk-build TARGET_LINK=1 NDK_DEBUG=1 CONFIGURATION=DynamicDebug
rmdir /s /q libs
rmdir /s /q obj
rem xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\arm64-v8a\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\arm64-v8a\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\armeabi\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\armeabi\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\armeabi-v7a\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\armeabi-v7a\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\mips\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\mips\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips64\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\mips64\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips64\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\mips64\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\x86\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\x86\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\StaticRelease\lib219EDB626EF14B42BE16F93A566F1CC3.a %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\x86_64\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\DynamicRelease\lib219EDB626EF14B42BE16F93A566F1CC3.so %ECO_FRAMEWORK%\Eco.ACOM2Python\BuildFiles\Android\x86_64\DynamicRelease\
pause
