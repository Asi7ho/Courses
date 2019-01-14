function ReadVideo(Movie, nb)

for k = 1:nb
    frame = Movie(k).cdata(:,:,1);
    imshow(uint8(frame));
end

end