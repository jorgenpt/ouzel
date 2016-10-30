
### Create a bare-bones Xcode project

1. Open Xcode and choose to create a new project
2. Pick OS X > Application > Cocoa Application as project template ![Project template choice](1_cocoa_application.png)
3. Pick a name, and uncheck all the project settings ![Project settings](2_project_settings.png)
4. Choose a directory and select that you'd like to create a git repository ![Choosing to create a git repository](3_create_git_repository.png)
5. Delete things we don't need
    * Delete ("move to trash") AppDelegate.h and AppDelegate.m, these will be provided by Ouzel
    * Delete ("move to trash") MainMenu.xib
    * Delete ("move to trash") Supporting Files, including main.m
    * This should leave your project looking like this ![Clean project overview](4_after_deleting_initial_content.png)

### Download ouzel and dependencies

Open a Terminal, and go to your project directory.

1. Set up a gitignore that deals with Xcode noise
    * `curl -o .gitignore https://raw.githubusercontent.com/github/gitignore/master/Global/Xcode.gitignore && git add .gitignore`
2. Add ouzel to your project directory
    * `git submodule add git@github.com:elnormous/ouzel.git ouzel`
3. Download dependencies for ouzel
    * `git submodule update --init --recursive`

![Terminal output after setup](5_after_cli_setup.png)

### Configure your project to build with ouzel

Return to Xcode, and resume setup there.

1. Add ouzel to your project
    1. Right click your project in the sidebar
    2. Choose 'Add Files to "YourOwnOuzelProject"...'
    3. Navigate to ouzel/build, and choose ouzel.xcodeproj
2. Add a barebones ouzel entrypoint
    1. Right click the "YourOwnOuzelProject" directory in the left sidebar
    2. Click "New File..."
    3. Choose C++ File under OS X > Source
    4. Name it YourOwnOuzelProject, and uncheck "Also create a header file".
    5. Add the following content:

        ```cpp
        #include "ouzel.h"
        ouzel::Engine engine;
        void ouzelMain(const std::vector<std::string>& args)
        {
            ouzel::Settings settings;
            settings.size = ouzel::Size2(640.0f, 480.0f);
            engine.init(settings);
        }
        ```

3. Select your project in the left sidebar and clear "Main Interface" under General > Deployment Info
4. Still in your project, under General > Linked Frameworks and Libraries, hit the plus, and add the following:
    * libouzel_macos.a
    * AudioToolbox.framework
    * Cocoa.framework
    * CoreVideo.framework
    * GameController.framework
    * Metal.framework
    * MetalKit.framework
    * OpenAL.framework
    * OpenGL.framework
5. Under Build Settings > Search Paths, add `ouzel/ouzel` to Header Search Paths

![After setting up dependencies](6_after_setting_up_dependencies.png)
![After setting up include path](7_after_setting_up_include_path.png)

You should now be able to build and run! ![Initial window](8_initial_run.png)
