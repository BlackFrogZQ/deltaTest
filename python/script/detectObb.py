from ultralytics import YOLO
import cv2
import numpy as np

resultImages = np.matrix([[1, 2], [3, 4]]) # 定义一个二维数组的全局变量

def packageRecognition(p_image):
    model = YOLO("./python/script/yolo11n-obb.pt")
    model = YOLO("./python/script/package.pt")
    image = cv2.cvtColor(p_image, cv2.COLOR_RGB2BGR)
    results = model(image, conf = 0.9, save = False)

    global resultImages
    resultImages = results[0].plot()

    detections = results[0].obb.xyxyxyxy
    num = len(detections)
    point_x = -1
    point_y = -1
    if num > 0:
        targetPoint = 0
        maxPonit = 0
        for i in range(num):
            point_x = int(results[0].obb.xywhr[i][0])
            point_y = int(results[0].obb.xywhr[i][1])
            if point_x > maxPonit:
                maxPonit = point_x
                targetPoint = i
        point_x = float(results[0].obb.xywhr[targetPoint][0])
        point_y = float(results[0].obb.xywhr[targetPoint][1])
    numInt = int(num)
    return (point_x, point_y, numInt)


def getResultImages():
    return resultImages
