# BCI UE Logger Lib

This library provides macros for additional logging features to UE_LOG and UE_LOGFMT.

* Logging with Context
* Include `__FILE__` and `__LINE__` by default
* Optional Structured Logging

After processing the logging values, the library forwards the results to a customizable log macro destination configured with the `BCICORELOG_FORWARD_TO` directive. Typically, this direct evaluates to `UE_LOG`. However by leaving this customizable, developers can chain together custom logging macros in case they've written any.

## BCILOG

This macro is intended to be the one most commonly used. It follows a similar structure as UE_LOG, except that instead of taking a format string with specifiers, it takes key/value pairs as an optional context payload. 

```
BCILOG(LOGCATEGORY, VERBOSITY, MESSAGE, CONTEXT);
```

For example, this is a valid BCILOG

```
FString ExampleContextValue = "Value";
BCILOG(LogTemp, Log, "Hello, world!", { "exampleContextKey", ExampleContextValue }, { "numbersAreSupported", 12345 });
```

Outputs:

```
[2025.04.29-23.36.00:177][658]LogTemp: [BCILOG] H:\BCITestProject\Plugins\BCI\BCICore\Source\BCICoreEditor\BCICoreEditor.cpp:[28]: Hello, world! - Context: { "exampleContextKey": "Value", "numbersAreSupported": "12345" }
```

Context values are formatted into JSON objects to make it easy to parse or transform programmatically.

## BCILOG_VAR

This macro is a special case and should be used sparingly. It supports discovering the verbosity from the current level of the category. This is meant to be used in the rare circumstance that the desired verbosity of the message is unknown until runtime. The intended use case is to call BCILOG from an arbitrary blueprint graph.

```
void UBCICoreLogLibrary::Log(const UObject* WorldContextObject, const FString& InFunction, const FString& InString)
{
	BCILOG_VAR(LogBlueprintUserMessages, InString, {"inFunction", InFunction});
}
```

## Optional: Structured Logging with spdlog

The game can consume spdlog as a ThirdParty plugin and set the preprocessor directive `WITH_SPDLOG` to true in order to support daily rotating structured logging. An OutputDevice will be created that hooks into `GLog` and forwards everything to `spdlog`. This means that not only with BCILOG statements appear in the regular UE_LOG outputs, all logging will be captured at least partially in spdlog files.

For example, the log line above would appear in `spdlog_2025-04-29.txt` as

```
{"timestamp":"2025-04-29T15:34:20.582Z","level":"info","logCategory":"LogTextureFormatOodle","verbosity":"Display","function":{"name":"","line":0},"message":"Oodle Texture loading DLL: oo2tex_win64_2.9.11.dll","context":{}}
{"timestamp":"2025-04-29T15:34:20.379Z","level":"info","logCategory":"LogTemp","verbosity":"Log","function":{"name":"H:\BCITestProject\Plugins\BCI\BCICore\Source\BCICoreEditor\BCICoreEditor","line":29},"message":"Hello, world!","context":{"exampleContextKey": "Value", "numbersAreSupported": "12345"}}
{"timestamp":"2025-04-29T15:34:20.613Z","level":"info","logCategory":"LogChaosDD","verbosity":"Log","function":{"name":"","line":0},"message":"Creating Chaos Debug Draw Scene for world Untitled","context":{}}
```

This provides a completely structured logging stream where every line is consumable as a JSON object. Additionally, the rotating logger collects logs across all play sessions in a game instead of a single file for each session.

## Onboarding

To onboard with the logging library, developers can incorporate the BCICore plugin into their projects: https://github.com/BirdCatcherGamesOrg/bci-ue-core-plugin

Alternatively, because the overhead of providing an entire Plugin just for a logging library is high, instead it can be consumed and placed into any module owned by the game team. To integrate the module, set the following values in the Build.cs file

```
if (Target.OptionalPlugins.Contains("spdlog"))
{
    bEnableExceptions = true;
    PublicIncludePaths.Add(spdlog.IncludePath(Path.Combine(PluginDirectory, "../../ThirdParty")));
    PublicDefinitions.Add("WITH_SPDLOG");
    PublicDependencyModuleNames.Add("spdlog");
}

PublicDefinitions.Add("INNER_LOGGING_CATEGORY=" + INNER_LOGGING_CATEGORY_MACRO);
PublicDefinitions.Add("BCICORELOG_FORWARD_TO=" + LOG_FORWARD_MACRO);
PublicDefinitions.Add("DLLEXPORT_API=" + LOGGING_DLLEXPORT_MACRO);
```

```
public static string INNER_LOGGING_CATEGORY_MACRO = "LogBCICore";
public static string LOGGING_DLLEXPORT_MACRO = "BCICORE_API";
public static string LOG_FORWARD_MACRO = "UE_LOG";
```

To customize these values, set the static strings from the Target file

```
THEMODULE.LOG_FORWARD_MACRO = "CUSTOM_LOG";
```

Lastly, see issue https://github.com/BirdCatcherGamesOrg/bci-ue-logger-lib/issues/1 to track the progress of creating a branch which provides the module as a standalone plugin.
