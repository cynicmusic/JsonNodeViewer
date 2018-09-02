# JsonNodeViewer
View, Move and Re-Parent JSON nodes in QT 5, save output JSON

![alt text](https://raw.githubusercontent.com/cynicmusic/JsonNodeViewer/master/image_screenshot.png)

## Build
Built on *Windows with Qt 5.3.0 and MinGW 4.8.2 32bit*. Open the .pro file with QtCreator and add a build kit (Qt 5 or later) to the project. I left my personal settings in the project (JsonNodeViewer.pro.user) which contains a custom build step. This step copies json.txt to the build folder.

If the .pro.user file does not work for you, the custom build step is as follows:
* Command: cmd.exe
* Arguments: /c copy /y "%{sourceDir}\json.txt" "%{buildDir}\json.txt
* Working Dir: %{buildDir}

![alt text](https://raw.githubusercontent.com/cynicmusic/JsonNodeViewer/master/image_custom_step.png)

## Unit Test
* In Qt Creator, select Projects, and clone the debug build
* Change the build directory to something unique, (e.g. by adding -UnitTest to directory name)
* Under Build Steps -> Additional arguments, enter "CONFIG +=test" (including the quotes) 

## Usage
* On launch, *json.txt* is loaded from the working directory.
* Drag nodes to reorder or reparent them. You must drop the nodes ON TOP of existing nodes, and not between them. This behavior could be improved.
* Click the *Save Document* button to save the JSON to *output.txt* to the working directory.
