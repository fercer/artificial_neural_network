library(datasets)
data(iris)

write.table(iris, 'iris.dat', row.names=F,quote=F,col.names=T)
