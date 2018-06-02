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
fact.a <- 1.5
fact.b <- 2.5
fact.c <- 0.5
fact.d <- 1.0
intercept <- -1.5
n.data <- 10000

x <- seq(-10, 10, length.out = n.data)
y <- fact.a * cos(fact.b * x) + fact.c * sin(fact.d * x) + intercept

plot(x, y, type="l")

write.table(x = cbind(x, y), file = "D:/Apps/artificial_neural_network/gm_testing/lm_data.dat", quote=F, col.names = F, row.names = F)


# Non-linear - test results
fact.a <- 1.499952
fact.b <- 2.500239
fact.c <- 0.008709
fact.d <- -4.625870
intercept <- -1.500464
y.tst <- fact.a.tst * cos(fact.b.tst * x) + fact.b.tst * sin(fact.a.tst * x) + intercept.tst

lines(x, y.tst, lty=3)
