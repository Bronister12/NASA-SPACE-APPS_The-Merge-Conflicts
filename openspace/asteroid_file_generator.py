import csv

def generate_asteroid_files(name, data):
    """
    Generate {name}.asset and {name}.csv files.

    data should be a dict with keys:
    full_name, epoch_cal, e, a, i, om, w, ma, per
    """

    # Asset file content
    asset_content = f'''local name = "{name}"

local transforms = asset.require("scene/solarsystem/sun/transforms")

local csvPath = asset.resource(name .. ".csv")

local Object = {{
  Identifier = name,
  Parent = transforms.SunEclipJ2000.Identifier,
  Renderable = {{
    Type = "RenderableOrbitalKepler",
    Path = csvPath,
    Format = "SBDB",
    Segments = 2000,
    SegmentQuality = 3,
    Color = {{ 0.98, 0.09, 0.06 }},
    TrailFade = 28,
    PointSizeExponent = 9.2
  }},
  GUI = {{
    Name = name,
    Path = "/Solar System/Small Bodies",
    Focusable = false
  }}
}}

asset.onInitialize(function()
  openspace.addSceneGraphNode(Object)
end)

asset.onDeinitialize(function()
  openspace.removeSceneGraphNode(Object)
end)

asset.export(Object)
'''

    # Write CSV file manually to ensure quoted fields
    csv_fields = ["full_name", "epoch_cal", "e", "a", "i", "om", "w", "ma", "per"]
    csv_filename = f"{name}.csv"
    with open(csv_filename, "w", newline="") as f:
        # Write header row with quotes
        f.write(",".join(f'"{field}"' for field in csv_fields) + "\n")

        # Write the single data row
        row_values = []
        for key in csv_fields:
            value = data[key]
            if key == "full_name":
                row_values.append(f'"{value}"')
            else:
                row_values.append(str(value))
        f.write(",".join(row_values) + "\n")

    # Write asset file
    with open(f"{name}.asset", "w") as f:
        f.write(asset_content)

    print(f"Generated {name}.asset and {name}.csv")


# Example usage
if __name__ == "__main__":
    example_data = {
        "full_name": "1566 Icarus (1949 MA)",
        "epoch_cal": "2024-03-31.0000000",
        "e": 0.826921456321578,
        "a": 1.078088223013253,
        "i": 22.80167079081121,
        "om": 87.9532449053327,
        "w": 31.43692554695528,
        "ma": 344.7553075375808,
        "per": 408.8649475991383
    }

    generate_asteroid_files("Icarus_test", example_data)
