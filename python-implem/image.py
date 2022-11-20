import cv2
import matplotlib.pyplot as plt
from skimage import io

def main():
    image = cv2.imread("../data/johnathan.png")
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    res = cv2.imread("../data/results/johnathan.png")
    io.imshow(res)
    plt.show()
    gray_blur = cv2.medianBlur(gray, 3)
    io.imshow(gray_blur)
    plt.show()


if __name__ == "__main__":
    main()