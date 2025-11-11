from PIL import Image
import os

TARGET_SIZE = (600, 600)  # Choose any consistent size you want

folder = "assets/sprites"

for filename in os.listdir(folder):
    if filename.endswith(".png"):
        path = os.path.join(folder, filename)
        img = Image.open(path)

        # Resize while keeping transparency
        img = img.resize(TARGET_SIZE, Image.LANCZOS)

        # Save back
        img.save(path)
        print("Resized", filename)

print("\n✅ All sprites normalized to", TARGET_SIZE)
