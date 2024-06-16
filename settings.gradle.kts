pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "Hello"
include(":HelloJNI")
include(":HelloFFmpeg")
include(":HelloFFmpeg:ffmpeg_lib")
include(":HelloOpenGLES")
include(":HelloFlutter")
include(":HelloFlutter:hello_flutter")
include(":HelloJNI:lib")
include(":HelloAIDL")
include(":HelloAIDL:libaidl")
