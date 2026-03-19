# Image To Objects - Geometry Dash Mod

A Geometry Dash mod that allows you to import images (JPG/PNG) into the level editor and convert them into colored square objects.

## Features

- Import any JPG or PNG image directly into the editor
- Optional downsampling to reduce object count
- Configurable block size for the generated objects
- Each pixel becomes a colored square block in the level
- Full undo support for easy removal

## Installation

### Prerequisites

1. Install [Geode](https://geode-sdk.org/) - the mod loader for Geometry Dash
2. Install [Geode CLI](https://docs.geode-sdk.org/getting-started/installation) for building mods

### Building from Source

1. Clone this repository:
```bash
git clone <repo-url>
cd gd-image-to-objects
```

2. Install the Geode SDK:
```bash
geode sdk install
```

3. Build the mod:
```bash
geode build
```

4. The built `.geode` file will be in the `build/` directory

## Usage

1. Open Geometry Dash with Geode installed
2. Create or open a level in the editor
3. Look for the green upload button in the editor toolbar (or in BetterEdit's toolbar)
4. Click to open the Image To Objects popup
5. Select your image file (JPG or PNG)
6. Set target dimensions (the image will be scaled down)
7. Set block size (default 30 is good for most images)
8. Click "Import Image" to generate the objects

### Tips

- **Smaller dimensions = fewer objects**: Start with 30-50 pixels for each dimension
- **Smaller block size = more detail but more objects**: 15-30 is usually best
- **High contrast images work best**: Simple logos and icons look great
- **Undo is your friend**: If it creates too many objects, press Ctrl+Z

## Configuration

You can modify default settings in `mod.json`:

- `default-block-size`: Default size for generated blocks (10-100)
- `default-target-width`: Default target width for image scaling (10-200)

## Technical Details

- Built with Geode SDK v5.0.0
- Supports Windows, Mac, Android, and iOS
- Uses CCImage for image loading
- Creates standard square blocks (GameObjectSize::kGameObjectSize32)

## License

MIT License - feel free to modify and distribute!

## Contributing

Pull requests welcome! Please follow Geode coding conventions.

## Acknowledgments

- [Geode SDK](https://github.com/geode-sdk/geode) - The Geometry Dash modding framework
- Geometry Dash community for modding resources
