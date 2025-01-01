![alt text](<Pasted image 20241231184039.png>)
The Bresenham algorithm is specifically designed to work on lines that go downwards (technically its still upwards its just that the way pixel indexing on displays work) with a slope less than 1. This may seem pretty limiting at first but we will extend this method to draw lines at any angle.
## Derivation

![alt](<Pasted image 20241231185348.png>)

Lets start at $(x_0, y_0)$ such that $f(x_0, y_0) = 0$ and since A, B and C are all integers in the line equation of our choice (they are calculated using start and end point using slopes and so on, you get the idea), so is $x_0$ and $y_0$ and is represented by the yellow dot. The intersection of the integral grid lines represent the pixels on the screen.

We move to the right in discrete steps incrementing the value of x. Now we need to decide whether we should colour $(x_0 + 1, y_0)$ or $(x_0 + 1, y_0 + 1)$ we do not need to consider the 8 total vertices since we have limited our scope to lines going downwards with a slope less than 1, that is, it can ONLY be one of those two points.

Rather intuitively we should choose the point that is closest to line. To do this we evaluate the line at the midpoint of both the purple dots i.e $f(x_0+1, y_0 + \dfrac{1}{2})$ 

If this value is positive, then the midpoint lies above the ideal line and lower purple point $(x_0+1, y_0+1)$is closer and vice versa.

We make the chosen purple point the yellow point and repeat.

## Algorithm for Integer Arithmetic

Apart from the 1/2 in the step where we check if either point is closer to the ideal line or not, the entire algorithm presented above uses only integers, if we could come up with a way to get rid of that step to figure out which point is closer to the ideal we could use integer-only arithmetic which computers are significantly faster at.

Lets define a difference $D_i$ as follows
$$
\Delta D_i = f(x_i + 1, y_i + \frac12) - f(x_i, y_i)
$$
Now for the first time we do this $f(x_0, y_0)$ is obviously going to be zero $D_0$ is nothing but the midpoint like we discussed above, and we proceed exactly like we did above as well. But for all subsequent decisions we need not compute the value of the line equation at the point. 

If $(x_0 + 1, y_0)$ is chosen then the change in the value of $D_i$ when it becomes $D_{i+1}$ will be
$$
\Delta D = A[x_0 + 2] + B[y_0 + \frac12] + C - (A[x_0 + 1] + B[y_0 + \frac12] + C)
$$
Which simplifies to A and similarly if we choose $(x_0 + 1, y_0+1)$ the change in D simplifies to A + B. We add this to the value of $D_i$ and check if it is positive or negative and proceed as usual.

Now A and B are known to be $\Delta y$ and $-\Delta x$ from the endpoints of the line since that is how we obtain our equation for the line. Moreover, the $\frac12$ in the value of $D_0$ isn't an integer, but since we only care about the sign of the value of $D_i$ we can multiply the whole process with 2 and nothing will change.

We finally arrive at the following pseudo-code:

```
plotLine(x0, y0, x1, y1)
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy
```

### But what is $D_i$?

$\Delta D_i$ is merely by how much the value of the midpoint calculation changes every time we move repeat this iteration, whereas the constant D itself refers to same evaluation at the midpoint operation that we did before to decide which point needs to be selected.

although later sections cover this it is fruitful to think about how this would change in the case where the slope is negative, i.e, we have to choose between the points $(x_0 + 1, y_0)$ and $(x_0 + 1, y_0 - 1)$ with the midpoint now being $(x_0+1, y_0 - \dfrac{1}{2})$. Giving us the following:
$$
\Delta D_i = f(x_i + 1, y_i - \frac12) - f(x_i, y_i)
$$
If $(x_0 + 1, y_0)$ is chosen then the change in the value of $D_i$ when it becomes $D_{i+1}$ will be
$$
\Delta D = A[x_0 + 2] + B[y_0 - \frac12] + C - (A[x_0 + 1] + B[y_0 - \frac12] + C)
$$
Which again evaluates to $\Delta y$

If $(x_0 + 1, y_0 -1)$ is chosen (when D > 0) then the change in the value of $D_i$ when it becomes $D_{i+1}$ will be
$$
\Delta D = A[x_0 + 2] + B[y_0 - \frac32] + C - (A[x_0 + 1] + B[y_0 - \frac12] + C)
$$
Which evaluates to $-\Delta y - \Delta x$
## Extending it to any angle

![alt ](<Pasted image 20250101152019.png>)

Right now the algorithm only works in octant 1 (the diagram is using regular coordinates instead of the upside coordinate system on screens)

Lets consider the case where the slope is $>1$ the algorithm is identical if we iterate over y instead of iterating over x, and we also need to change the second point of consideration from $(x_0 + 1, y_0)$ to $(x_0, y_0 + 1)$. Moreover the midpoint of the two points we have to choose from changes to $(x_0 + \frac12, y_0+1)$. All of these changes are immediately handled if we just swap the variables $x_0$ and $y_0$ and $x_1$ and $y_1$ and plot y, x instead of x, y since the variables are now inverted. Don't take my word for it, try it and see.

```
plotLine(x0, y0, x1, y1)
	steep = false
	if abs(y1 - y0) > abs(x1 - x0):
		swap(x0, y0)
		swap(x1, y1)
		steep = true
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy
```

![](<Pasted image 20250101160933.png>)

Now we have octant 1 and 2 done, what about octant 5 and 6. The above diagram shows us whats different between the two quadrants. Notice that the only difference in octant 5 and 6 is the direction in which our lines are being drawn, if we just swap the points whenever $x_0 > x_1$ we get what we are looking for. Should we be doing this after or before the swap in case the slope is greater than 1? We should be doing this after we decide to swap variables for greater slopes. Because what we are doing right now is changing the order of the variables we are meant to be looping over, and the the swap for the loops is deciding *which* variable *x or y* we need to be looping over.

```
plotLine(x0, y0, x1, y1)
	steep = false
	if abs(y1 - y0) > abs(x1 - x0):
		swap(x0, y0)
		swap(x1, y1)
		steep = true
	if x0 > x1:
		swap(x0, x1)
		swap(y0, y1)
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy
```

What about cases when the slope is negative, which is what happens in octants 7 and 8? Going back to diagram with yellow and green dots this refers to the case where the line is sloping upwards and we now have to choose between the points $(x_0 + 1, y_0)$ and $(x_0 + 1, y_0 - 1)$ with the midpoint now being $(x_0+1, y_0 - \dfrac{1}{2})$ the difference function changes and if the value evaluated at this point is positive then we choose $(x_0 + 1, y_0 - 1)$. This can be fixed if we decrease the value of y instead of increase it and change the dy we use in the Difference equation to -dy.

```
plotLine(x0, y0, x1, y1)
	steep = false
	if abs(y1 - y0) > abs(x1 - x0):
		swap(x0, y0)
		swap(x1, y1)
		steep = true
	if x0 > x1:
		swap(x0, x1)
		swap(y0, y1)
    dx = x1 - x0
    dy = y1 - y0
    yi = 1
    if dy < 0:
	    yi = -1
	    dy = -dy
    D = 2*dy - dx
    y = y0
    
    for x from x0 to x1
        plot(x, y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy
```

The changes that we made to plot octants 5 and 6 still apply as its a valid operation for the entire left half of the plane and by making these changes octants 3 and 4 get covered on their own with no further changes.