library(datasets)
data(iris)

all.idx <- 1:150
training.data.idx <- sample(all.idx,100,F)
testing.data.idx <- all.idx[-training.data.idx]

training.data <- iris[training.data.idx,]
testing.data <- iris[training.data.idx,]

write.table(training.data, 'training.dat', row.names=F,quote=F,col.names=T)
write.table(testing.data, 'testing.dat', row.names=F,quote=F,col.names=T)


