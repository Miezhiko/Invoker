if not exist "build" mkdir "build"
cd "build"

cmake -DCMAKE_BUILD_TYPE=Release -DUSE_LTO=ON ..
"C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" Invoker.sln /p:configuration=Release /p:platform=x64
