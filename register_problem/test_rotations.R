setwd('D:/Documentos/development/testing_ann/testing_register')

UL <- c(-100, 50)
UR <- c(50, 50)
LR <- c(50, -75)
LL <- c(-100, -75)
corners <-rbind(UL, UR, LR, LL)

c.x <- (UL[1] + LR[1]) / 2
c.y <- (UL[2] + LR[2]) / 2

theta <- 30.0 / 180.0 * pi
phi <- theta

rot.mat <- matrix(c(cos(theta), sin(phi), -sin(theta), cos(phi)), ncol = 2, byrow=T)
rot.mat

inv.rot.mat <- matrix(c(cos(phi), -sin(phi), sin(theta), cos(theta)), ncol=2, byrow=T) / (cos(theta)*cos(phi) + sin(theta)*sin(phi))
inv.rot.mat

r.corners <- corners %*% rot.mat
i.corners <- r.corners %*% inv.rot.mat

corners
r.corners
i.corners
c.x
c.y

plot(r.corners, pch = 15:18, col='green')
points(corners, pch=15:18, col = 'blue')
points(i.corners, pch=3, col = 'yellow')
points(x = c.x, y = c.y, pch=3, col='blue')

max_height = 585
max_width = 607

half_src_width = 303
half_max_width = 303
half_src_height = 292
half_max_height = 292

parameter_A = 1
parameter_B = 0
parameter_C = 0
parameter_D = 1

i <- matrix(rep(seq(1:max_height),max_width), nrow = max_width) 
j <- seq(1:max_width)
j.m <- rep(j, max_height)



