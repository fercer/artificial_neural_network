img = single(imread('img_target.pgm')) / 255.0;

[h, w] = size(img)


Gx_x = [-1 0 1];
Gx_y = [1;2;1];
Gx = conv2(Gx_y, Gx_x, 'full')

Gy_x = [1 2 1];
Gy_y = [-1;0;1];
Gy = conv2(Gy_y, Gy_x, 'full')

resp_Gx = conv2(img, Gx, 'same');
resp_Gy = conv2(img, Gy, 'same');

imshow(resp_Gy, [])
imshow(resp_Gx, [])

nearest_2p_dim = 2^(ceil(log2(max(h, w))));

[kw, kh] = size(Gx)
offset_kx = floor(kw/2)
offset_ky = floor(kh/2)

img_zp = zeros(nearest_2p_dim, nearest_2p_dim);
kernel_zp = zeros(nearest_2p_dim, nearest_2p_dim);

img_zp((1:h), (1:w)) = img;
kernel_zp(1:kh, 1:kw) = Gx;

fft_img = fft2(img_zp);
fft_kernel = fft2(kernel_zp);
fft_resp = fft_kernel .* fft_img;
resp = ifft2(fft_resp);
imshow(resp, [])
filter_resp = resp((1:h)+offset_ky, (1:w)+offset_kx);

sum(sum((filter_resp - resp_Gx).^2))
imshow(filter_resp - resp_Gx)

