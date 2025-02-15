---
sidebar_position: 1
---

# Intro to Blueprint Editing

## Requirements

- You've done everything in [Getting Started](../../gettingstarted.md) to get accustomed to Pal Schema.

- Pal Schema 0.2.0+ as this feature was added in 0.2.0.

- Must run Palworld once after installing Pal Schema to generate a [config.json](configuration.md) file.

## Infinite Sprint Stamina

Start by creating the following folder structure in your mods folder: `MyFirstBPMod\blueprints` and create a `sprint_stamina.json` file in the `blueprints` folder.

The folder **must** be called `blueprints` for Pal Schema to pick it up. `MyFirstBPMod` and `sprint_stamina` you can just call anything really.

![](assets/tutorial1_folderstructure.png)

Open `sprint_stamina.json` and Copy Paste the following in it:

```json
{
    "BP_PalGameSetting_C": {
        "SprintSP": 0.0
    }
}
```

`BP_PalGameSetting` is the name of the asset we want to edit and inside it is `SprintSP` which is a member variable of `BP_PalGameSetting` that controls the stamina consumption rate when sprinting. In this case we're setting it to 0.0 which means we'll be able to run forever without running out of stamina.

Notice how the asset name has `_C` at the end. Appending a `_C` to the end of the asset name is important because otherwise Pal Schema will ignore the entry.

`SprintSP` is one of the many variables available in `BP_PalGameSetting` and you can use the [LiveView](https://docs.ue4ss.com/dev/feature-overview/live-view.html) feature in UE4SS to inspect what variables each asset has. [FModel](https://fmodel.app/) works too, but it doesn't show everything since it only shows variables that have been changed from their defaults.

We're done here now. Save the file and navigate over to the `config` folder which is located in `ue4ss/Mods/PalSchema` folder assuming you're on version 0.2.0 of PalSchema or higher and you've run the game at least once after installing the newest version.

![](assets/config_file.png)

You'll need to open the config file and set `enableExperimentalBlueprintSupport` to `true`. This will enable support for loading blueprint editing mods in Pal Schema. This must also be set by anyone using a mod that edits blueprints.

```json
{
    "languageOverride": "",
    "enableExperimentalBlueprintSupport": true
}
```

Let's test it in-game now.

![](assets/tutorial1_finalresult.gif)

If everything was done correctly, you shouldn't see the stamina bar at all when running. Congratulations!