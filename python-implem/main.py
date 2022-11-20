import cv2
from cvzone.SelfiSegmentationModule import SelfiSegmentation

from skimage import io
from pykuwahara import kuwahara
import matplotlib.pyplot as plt


def reduce_val(val, div):
    if int(val) < 192:
        return (val / div + 0.5) * div
    return 255


def reduceImage(image, div):
    h, w, _ = image.shape
    for x in range(h):
        for y in range(w):
            image[x][y] = \
                [reduce_val(image[x][y][0], div),
                 reduce_val(image[x][y][1], div),
                 reduce_val(image[x][y][2], div)]
    return image


def edge_mask(img, line_size, blur_value):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gray_blur = cv2.medianBlur(gray, blur_value)
    edges = cv2.adaptiveThreshold(gray_blur, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, line_size, blur_value)
    return edges


def cartoonize(img):

    print("Compute object contour")
    edges = edge_mask(img, 7, 5)
    io.imshow(edges)
    plt.show()

    print("Reduce number of colors")
    kwh = reduceImage(img, 64)
    io.imshow(img)

    print("Applying Kuwahara Filter")
    kwh = kuwahara(img, method='mean', radius=5, sigma=3)
    io.imshow(kwh)
    plt.show()


    print("Draw object contour")
    cartoon = cv2.bitwise_and(kwh, kwh, mask=edges)
    return cartoon



def main():
    print("Load Data")
    img = cv2.imread("data/jules.dorbeau.png")
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    cartoon = cartoonize(img)

    io.imshow(cartoon)
    plt.show()
    cartoon = cv2.cvtColor(cartoon, cv2.COLOR_RGB2BGR)
    cv2.imwrite('data/results/data-res.jpg', cartoon)


if __name__ == "__main__":
    main()