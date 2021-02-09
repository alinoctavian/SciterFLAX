using Flax.Build;
using Flax.Build.NativeCpp;
using System.IO;

public class Game : GameModule
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // C#-only scripting
        BuildNativeCode = true;
    }

    /// <inheritdoc />
    public override void Setup(BuildOptions options)
    {
        base.Setup(options);

        options.ScriptingAPI.IgnoreMissingDocumentationWarnings = true;


        var sciterPath = Path.Combine(FolderPath, "..", "sciter");
        options.CompileEnv.IncludePaths.Add(Path.Combine(sciterPath, "include"));
        options.DependencyFiles.Add(Path.Combine(sciterPath, "bin", "sciter.dll"));
        options.CompileEnv.PreprocessorDefinitions.Add("WINDOWLESS");
        // Here you can modify the build options for your game module
        // To reference another module use: options.PublicDependencies.Add("Audio");
        // To add C++ define use: options.PublicDefinitions.Add("COMPILE_WITH_FLAX");
        // To learn more see scripting documentation.
    }
}
