call ndk-build clean
call ndk-build TARGET_LINK=0 NDK_DEBUG=0 CONFIGURATION=StaticRelease
call ndk-build TARGET_LINK=0 NDK_DEBUG=1 CONFIGURATION=StaticDebug
rmdir /s /q libs
rmdir /s /q obj
xcopy /y ..\..\..\..\BuildFiles\Android\arm64-v8a\StaticRelease\lib00000000000000000000000053595333.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\arm64-v8a\StaticRelease\
xcopy /y ..\..\..\..\BuildFiles\Android\armeabi\StaticRelease\lib00000000000000000000000053595331.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\armeabi\StaticRelease\
xcopy /y ..\..\..\..\BuildFiles\Android\armeabi-v7a\StaticRelease\lib00000000000000000000000053595332.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\armeabi-v7a\StaticRelease\
xcopy /y ..\..\..\..\BuildFiles\Android\mips\StaticRelease\lib00000000000000000000000053595331.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\mips\StaticRelease\
xcopy /y ..\..\..\..\BuildFiles\Android\mips64\StaticRelease\lib00000000000000000000000053595332.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\mips64\StaticRelease\
xcopy /y ..\..\..\..\BuildFiles\Android\x86\StaticRelease\lib00000000000000000000000053595332.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\x86\StaticRelease\
xcopy /y ..\..\..\..\BuildFiles\Android\x86_64\StaticRelease\lib00000000000000000000000053595333.a %ECO_FRAMEWORK%\Eco.System1\BuildFiles\Android\x86_64\StaticRelease\
pause
