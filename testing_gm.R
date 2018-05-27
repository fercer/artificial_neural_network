setwd('D:/Documentos/development/testing_ann/testing_gm')
set.seed(777)

mean.1.x <- 10.0
mean.1.y <- 0.0
sd.1 <- 1.0

mean.2.x <- 30.0
mean.2.y <- 5.0
sd.2 <- 2.0

class.1.x <- rnorm(100, mean.1.x, sd.1)
class.1.y <- rnorm(100, mean.1.y, sd.1)
class.1 <- cbind(class.1.x, class.1.y, 0)


class.2.x <- rnorm(150, mean.2.x, sd.2)
class.2.y <- rnorm(150, mean.2.y, sd.2)
class.2 <- cbind(class.2.x, class.2.y, 1)

plot(class.1[,-3], col = 'red', xlim = c(min(class.1.x, class.2.x), max(class.1.x, class.2.x)), ylim = c(min(class.1.y, class.2.y), max(class.1.y, class.2.y)))
points(class.2[,-3], col = 'blue')

dataset.all <- rbind(class.1, class.2)
dataset.all.size <- nrow(dataset.all)
dataset.all.idx <- 1:dataset.all.size

dataset.trn.size <- floor(0.6 * dataset.all.size)
dataset.trn.idx <- sample(dataset.all.idx, dataset.trn.size, F)

dataset.tst.size <- dataset.all.size - dataset.trn.size
dataset.tst.idx <- sample(dataset.all.idx[-dataset.trn.idx], dataset.tst.size, F)


dataset.trn <- dataset.all[dataset.trn.idx,]
dataset.tst <- dataset.all[dataset.tst.idx,]


plot(dataset.trn[dataset.trn[,3] == 0,-3], col = 'blue', xlim = c(min(class.1.x, class.2.x), max(class.1.x, class.2.x)), ylim = c(min(class.1.y, class.2.y), max(class.1.y, class.2.y)) )
points(dataset.trn[dataset.trn[,3] == 1,-3], col = 'red')

plot(dataset.tst[dataset.tst[,3] == 0,-3], col = 'blue', xlim = c(min(class.1.x, class.2.x), max(class.1.x, class.2.x)), ylim = c(min(class.1.y, class.2.y), max(class.1.y, class.2.y)) )
points(dataset.tst[dataset.tst[,3] == 1,-3], col = 'red')

write.table(dataset.all, 'synthethic.dat', quote = F, col.names = F, row.names = F)

weights <- c(  2.112711, -0.244944, -28.278207)

activations <- 1.0 / (1.0 + exp(- cbind(dataset.all[,-3], 1) %*% weights))

plot(class.1[,-3], col = 'red', xlim = c(min(class.1.x, class.2.x), max(class.1.x, class.2.x)), ylim = c(min(class.1.y, class.2.y), max(class.1.y, class.2.y)))
points(class.2[,-3], col = 'blue')
abline(b = -weights[1]/weights[2], a = (0.5 - weights[3])/weights[2])

library(rgl)
points3d(x = dataset.all[,1], y = dataset.all[,2], z = activations)
