import os
import cv2
from tqdm import tqdm

def split_images(input_folder="Input", output_folder="Output", tile_size=(50, 50)):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    image_files = [f for f in os.listdir(input_folder) if f.lower().endswith(('png'))]
    total_tiles = 0
    total_count = sum((cv2.imread(os.path.join(input_folder, f)).shape[0] // tile_size[1]) * 
                      (cv2.imread(os.path.join(input_folder, f)).shape[1] // tile_size[0])
                      for f in image_files if cv2.imread(os.path.join(input_folder, f)) is not None)
    
    progress_bar = tqdm(total=total_count, desc="Elaborazione immagini")
    
    for idx, image_file in enumerate(image_files):
        image_path = os.path.join(input_folder, image_file)
        image = cv2.imread(image_path)
        
        if image is None:
            print(f"Errore nel caricamento di {image_file}")
            continue
        
        img_height, img_width, _ = image.shape
        tile_w, tile_h = tile_size
        
        for row in range(0, img_height, tile_h):
            for col in range(0, img_width, tile_w):
                tile = image[row:row + tile_h, col:col + tile_w]
                
                if tile.shape[0] == tile_h and tile.shape[1] == tile_w:
                    file_name = f"{col//tile_w}_({(idx + 1)*100 + row//tile_h}).png"
                    tile_path = os.path.join(output_folder, file_name)
                    cv2.imwrite(tile_path, tile)
                    total_tiles += 1
                    progress_bar.update(1)
        
    progress_bar.close()
    print(f"Totale tiles create: {total_tiles}")

split_images()
