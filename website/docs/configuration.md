---
sidebar_position: 2
---

# Configuration

When you run Palworld, it'll generate a `config.json` in a config folder where PalSchema resides. This config file can be used to customize how PalSchema functions and more settings will be added as development progresses further.

I will present an example of a default config.json after it has been generated and explain what each value does.

```json
{
    "languageOverride": ""
}
```

`languageOverride` can be used to load custom translation mods for languages that the game doesn't normally support, this could in theory be anything, but it's preferred to follow the [ISO-639](https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes) two-letter codes. If it's a made up language like pirate speak for example then you can just do `pirate`.