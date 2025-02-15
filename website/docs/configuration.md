---
sidebar_position: 2
---

# Configuration

When you run Palworld, it'll generate a `config.json` in a config folder where Pal Schema resides. This config file can be used to customize how Pal Schema functions and more settings will be added as development progresses further.

I will present an example of a default config.json after it has been generated and explain what each value does.

```json
{
    "languageOverride": "",
    "enableExperimentalBlueprintSupport": false
}
```

`languageOverride` can be used to load custom translation mods for languages that the game doesn't normally support, this could in theory be anything, but it's preferred to follow the [ISO-639](https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes) two-letter codes. If it's a made up language like pirate speak for example then you can just do `pirate`. Setting `languageOverride` to `""` will default to using whatever language Steam is set to.

`enableExperimentalBlueprintSupport` is used to enable or disable experimental loading of blueprint editing mods. Defaults to `false`.