# plyRender

================================================================
Known Bugs:

Error Finding Centroid on Stanford Dragon (dragon_vrip_res2.ply)
	Object never even renders.
Centroid is (-nan,-nan,-nan)
Need error checking there

================================================================
More Realism:

Projection
	Right now translateing the 'z' axis doesn't
	appear to change anything until it starts 
	dissapearing because its moving past the camera.

Other Coloration Schemes?

Faces that are closer to the camera's Z should be "solid"
	Currently faces that are covered by other faces
	but have positive normals show through the "top
	layer."

================================================================
Efficiancy:

Long load times and lack of smooth motion with larger
files. System monitor shows CPU at 100%. There may be
calculations that the GPU could do faster but are being
execuded by the CPU.

I suspect there are certian calculations that I programed
innefficiantly. For example, my transform methods are
"hard coded" (I manually create the matricies) even though
OpenGL probably has pre-build functions that do the same
thing and probably do it faster.

================================================================
Little Things:

Better / More intuitive controls (User Interaction)
	
	A Key that tells the User what each control does

	Clockwise & Conterclockwise Rotation

	Advanced Mode that allows User to enter exact
	values for their movement.

	Mouse Controls? Use scroll wheel for scaling?

	Reset button that returns the render to original
	position

	Auto rotate option? For Demo purposes

	Let User change colors for background & render
