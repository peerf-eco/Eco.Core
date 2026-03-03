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
rem xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\arm64-v8a\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\arm64-v8a\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\armeabi\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\armeabi\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\armeabi-v7a\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\armeabi-v7a\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\mips\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\mips\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips64\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\mips64\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\mips64\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\mips64\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\x86\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\x86\DynamicRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\StaticRelease\libC8035C9741CB40F9B104A12C3F41E2AA.a %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\x86_64\StaticRelease\
rem xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\DynamicRelease\libC8035C9741CB40F9B104A12C3F41E2AA.so %ECO_FRAMEWORK%\Eco.ACOM2Java\BuildFiles\Android\x86_64\DynamicRelease\
pause
