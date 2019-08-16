### Science-fiction style image filtering
##### Honours Project by Walter Cheng
##### Supervisor: Professor David Mould

- ![](https://github.com/Walter0697/Science-Fiction-Style-Image-Filtering/blob/master/cover.png)

## Technology:
- OpenCV
- Visual Studio

## Description:
- A filtering algorithm for generating a science-fiction style image just like the cover image

## Dev Blog:
- https://walterpakwai.wixsite.com/sci-fi-filtering

## How to use:
- For more details on what the function is trying to achieve, see dev blog to know more
- Most of the variables you need to adjust will be in the main function
- uncomment imshow() function in order to show the specific progress
- changing the name of filename, refname, noisename will change the input image
- changing the name of funcname and outputname will change the output image
- changing the values in function colorPickByMidPoint() will change the main two colors
- adding the values in function colorPickByMidPoint() will change the gradient of the color picker
- adding the values in function fillColor() will change the value the ratio between the source image and the color picker
- changing the values in getMarkers() will change the size and area of the sections
- changing the number between 0 and contours.size in both getAlpha() function will get different area that you want to be lighten up
- setting the last boolean to true or false in finalCombine() will decide if you are using the noise image or not