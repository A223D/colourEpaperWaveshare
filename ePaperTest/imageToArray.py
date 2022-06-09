from PIL import Image

im = Image.open('./ditherDoraemon.png') #edit the name of the dithered image here
im = im.convert('RGBA')

pix = list(im.getdata())

print(len(pix))
#print(pix[0])

dataString = "#include \"imagedata.h\"\n#define PROGMEM_LATE __attribute__ (( __section__(\".fini1\") ))\nconst unsigned char image1[32000] PROGMEM_LATE={ "

def getCol(index):
    if(index == (0, 0, 0, 255)):
        return 0
    elif(index == (0, 0, 255, 255)):
        return 3
    elif(index ==(255, 0, 0, 255)):
        return 4
    elif(index == (0, 255, 0, 255)):
        return 2
    elif(index == (255, 128, 0, 255)):
        return 6
    elif(index == (255, 255, 0, 255)):
        return 5
    elif(index == (255, 255, 255, 255) or index[3]==0):     #the second condition maps transparency to black
        return 1
    else:
        print("Something went wrong with this pixel: ", index)
        quit()

# for i in range(0, 64):
#     print(pix[i])

for i in range(0, 64000, 2):
    a = getCol(pix[i])
    b = getCol(pix[i+1])
    dataString+="0x"
    dataString+=str(a)
    dataString+=str(b)
    if(i != 63998):
        dataString+=", "

dataString+="};\n"

# text_file = open("./images/f/image1.c", "w")
# n = text_file.write(dataString)
# text_file.close()
# print(n)

dataString += "const unsigned char image2[32000] PROGMEM_LATE={ "

for i in range(64000, 128000, 2):
    a = getCol(pix[i])
    b = getCol(pix[i+1])
    dataString+="0x"
    dataString+=str(a)
    dataString+=str(b)
    if (i != 127998):
        dataString+=", "
dataString+="};\n"

# text_file = open("./images/f/image2.c", "w")
# n = text_file.write(dataString)
# text_file.close()
# print(n)

dataString += "const unsigned char image3[32000] PROGMEM_LATE={ "

for i in range(128000, 192000, 2):
    a = getCol(pix[i])
    b = getCol(pix[i+1])
    dataString+="0x"
    dataString+=str(a)
    dataString+=str(b)
    if (i != 191998):
        dataString+=", "

dataString+="};\n"

# text_file = open("./images/f/image3.c", "w")
# n = text_file.write(dataString)
# text_file.close()
# print(n)

dataString += "const unsigned char image4[32000] PROGMEM_LATE={ "

for i in range(192000, 256000, 2):
    a = getCol(pix[i])
    b = getCol(pix[i+1])
    dataString+="0x"
    dataString+=str(a)
    dataString+=str(b)
    if (i != 255998):
        dataString+=", "
dataString+="};\n"

text_file = open("./imagedata.cpp", "w")
n = text_file.write(dataString)
text_file.close()
#print(n)
