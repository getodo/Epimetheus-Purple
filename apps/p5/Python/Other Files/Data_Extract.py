from typing import NewType
import pandas as pd
import numpy as np
import re
from sklearn.neighbors import KNeighborsClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier, VotingClassifier
from sklearn import metrics

df = pd.ExcelFile(
    r"C:\Users\Aiden\csse4011\csse4011-s4529666\TrainingDataProject.xlsx")

dfs = df.parse()
s = np.array((dfs['Accelerometor Data'].str.translate(str.maketrans({'[': '', ']': ''})).tolist()))
newArr = []

for row in s:
    t = row.split(',')
    t = np.array(t)
    t = t.astype(float)
    t = np.split(t, 2)
    newArr.append(t)

accelArr = np.array(newArr)
actionArr = np.array(dfs['Action'])

# Create KNN classifier
knn = KNeighborsClassifier(n_neighbors=3)
clf1 = LogisticRegression(multi_class='multinomial', random_state=1)
clf2 = RandomForestClassifier(n_estimators=50, random_state=1)
clf3 = GaussianNB()
eclf1 = VotingClassifier(estimators=[
...         ('lr', clf1), ('rf', clf2), ('gnb', clf3)], voting='hard')
eclf1 = eclf1.fit(accelArr, actionArr)

