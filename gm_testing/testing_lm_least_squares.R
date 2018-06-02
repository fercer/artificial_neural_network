# Mean squares testing - LM

# Linear - simple
slope <- 2.5
intercept <- 1.5
n.data <- 100

x <- seq(-10, 10, length.out = n.data)
y <- x * slope + intercept + rnorm(n.data, 0, 1.5)

plot(x, y)

write.table(x = cbind(x, y), file = "D:/Apps/artificial_neural_network/gm_testing/lm_data.dat", quote=F, col.names = F, row.names = F)



# Non-linear - simple
fact.a <- 1.0
fact.b <- 1.4
fact.c <- 1.0
fact.d <- 1.9
intercept <- -1.5
n.data <- 10000

x <- seq(-10, 10, length.out = n.data)
y <- fact.a * cos(fact.b * x) + fact.c * sin(fact.d * x) + intercept

plot(x, y, type="l")

write.table(x = cbind(x, y), file = "D:/Apps/artificial_neural_network/gm_testing/lm_data.dat", quote=F, col.names = F, row.names = F)


# Non-linear - test results
fact.a.tst <- 1.0
fact.b.tst <-  0.571005
fact.c.tst <- 1.0
fact.d.tst <- 1.903021
intercept.tst <- -1.330659
y.tst <- fact.a.tst * cos(fact.b.tst * x) + fact.c.tst * sin(fact.d.tst * x) + intercept.tst

lines(x, y.tst, lty=3)
