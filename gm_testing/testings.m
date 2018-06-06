img = single(imread('img_target_3.pgm')) / 255.0;
[h, w] = size(img)

Gx_x = [-1 0 0 1];
Gx_y = [1;2;1];
Gx = conv2(Gx_y, Gx_x, 'full')

Gy_x = [1 2 1];
Gy_y = [-1;0;1];
Gy = conv2(Gy_y, Gy_x, 'full')

resp_Gx = conv2(img, Gx, 'same');
resp_Gy = conv2(img, Gy, 'same');

imshow(resp_Gy, [])
imshow(resp_Gx, [])

kernel = Gx;

kernel = single(imread('img_target.pgm')) / 255.0;
%kernel = kernel(149:275, 314:480);
imshow(kernel, [])

[kh, kw] = size(kernel)

resp_kernel = conv2(img, kernel, 'same');
[max_val, max_idx] = max(resp_kernel(:));
max_x = floor(max_idx/h)
max_y =  mod(max_idx, h)
imshow(resp_kernel, [])
viscircles([max_x, max_y], 10)

offset_kx = floor(kw/2)
offset_ky = floor(kh/2)

nearest_2p_dim = 2^(ceil(log2(max([h + kh, w + kw]))));

img_zp = zeros(nearest_2p_dim, nearest_2p_dim);
kernel_zp = zeros(nearest_2p_dim, nearest_2p_dim);

img_zp(1:h, 1:w) = img;
kernel_zp(1:kh, 1:kw) = kernel;

tic
fft_img = fft2(img_zp);
toc

fft_kernel = fft2(kernel_zp);
fft_resp = fft_kernel .* fft_img;
resp = ifft2(fft_resp);
filter_resp = resp((1:h)+offset_ky, (1:w)+offset_kx);

figure
subplot(1, 2, 1)
imshow(filter_resp, [])
[max_val, max_idx] = max(filter_resp(:));
max_x = floor(max_idx/h)
max_y =  mod(max_idx, h)
viscircles([max_x, max_y], 10)

subplot(1, 2, 2)
imshow(resp_kernel, [])
[max_val, max_idx] = max(resp_kernel(:));
max_x = floor(max_idx/h)
max_y =  mod(max_idx, h)
viscircles([max_x, max_y], 10)

sum(sum((filter_resp - resp_kernel).^2))
imshow(filter_resp - resp_kernel)

