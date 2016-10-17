function demo(im)
if size(im, 3) == 3
    im = rgb2gray(im);
end
det = detect_apriltag_matlabmex(im'); % MATLAB stores images as transpose of form expected
clf reset
imshow(im);
hold on;
X = det([1 3 5 7 1],:);
Y = det([2 4 6 8 2],:);
h = plot(X, Y, '-', 'LineWidth', 3);
for a = 1:size(det, 2)
    text(mean(X(1:4,a)), mean(Y(1:4,a)), sprintf('%d', det(9,a)), 'FontSize', 32, 'Color', get(h(a), 'Color'), 'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle');
end
end