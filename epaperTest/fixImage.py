from PIL import Image

im = Image.open('./doraemon.png')
im = im.convert('RGB')


px = im.load()

uniqueColourList=[]

for i in range(0, im.width):
    for j in range(0, im.height):
        if px[i, j] not in uniqueColourList:
            uniqueColourList.append(px[i, j])
            
        if px[i, j][0] >=199 and px[i, j][1] >=199 and px[i, j][2] >=199:
            px[i, j] = (255, 255, 255)
            
            
im.save('./newDoraemon.png')
        
print(len(uniqueColourList))

uniqueColourList=[]
for i in range(0, im.width):
    for j in range(0, im.height):
        if px[i, j] not in uniqueColourList:
            uniqueColourList.append(px[i, j])
            
print(len(uniqueColourList))