#version 450

out vec4 vFragColor;

void main(){
	vec4 color_left = vec4(1.f, 0.f, 0.f, 1.f);
	vec4 color_top = vec4(0.f, 1.f, 0.f, 1.f);
	vec4 color_right = vec4(0.f, 0.f, 1.f, 1.f); 

	float width = 1366;
	float height = 768;
	float x = gl_FragCoord.x;
	float y = gl_FragCoord.y;
	
	// Vectors to middle
	float left_corner_to_middle_vector_x = width/2;
	float left_corner_to_middle_vector_y = height/2;
	float right_corner_to_middle_vector_x = -width/2;
	float right_corner_to_middle_vector_y = height/2;
	float top_corner_to_middle_vector_x = 0;
	float top_corner_to_middle_vector_y = -height/2;
	
	// Corner points
	float left_corner_x = 0;
	float left_corner_y = 0;
	float right_corner_x = width;
	float right_corner_y = 0;
	float top_corner_x = width/2;
	float top_corner_y = height;
	
	// lengths of vectors
	float length_of_left_corner_to_middle = 783.54;
	float length_of_right_corner_to_middle = 783.54;
	float length_of_top_to_middle = 384;

	// normed vectors
	float normed_left_corner_to_middle_vector_x = left_corner_to_middle_vector_x / length_of_left_corner_to_middle;
	float normed_left_corner_to_middle_vector_y = left_corner_to_middle_vector_y / length_of_left_corner_to_middle;
	float normed_right_corner_to_middle_vector_x = right_corner_to_middle_vector_x / length_of_right_corner_to_middle;
	float normed_right_corner_to_middle_vector_y = right_corner_to_middle_vector_y / length_of_right_corner_to_middle;
	float normed_top_corner_to_middle_vector_x = top_corner_to_middle_vector_x / length_of_top_to_middle;
	float normed_top_corner_to_middle_vector_y = top_corner_to_middle_vector_y / length_of_top_to_middle;
	
	// relative x and y
	float x_left_corner_rel = x;
	float y_left_corner_rel = y;
	float x_top_corner_rel = x - top_corner_x;
	float y_top_corner_rel = y - top_corner_y;
	float x_right_corner_rel = x - right_corner_x;
	float y_right_corner_rel = y - right_corner_y;

	// inner products
	float innerproduct_red = x_left_corner_rel * normed_left_corner_to_middle_vector_x + y_left_corner_rel * normed_left_corner_to_middle_vector_y;
	float innerproduct_green = x_top_corner_rel * normed_top_corner_to_middle_vector_x + y_top_corner_rel * normed_top_corner_to_middle_vector_y;
	float innerproduct_blue = x_right_corner_rel * normed_right_corner_to_middle_vector_x + y_right_corner_rel * normed_right_corner_to_middle_vector_y;

	// colorgradient
	// note that red means bottom right corner, green top corner and blue is bottem left corner.
	float colorgradient_red = +0.05 + 1 - (innerproduct_red/length_of_left_corner_to_middle);
	float colorgradient_green = +0.05 + 1 - (innerproduct_green/length_of_top_to_middle);
	float colorgradient_blue = +0.05 + 1 - (innerproduct_blue/length_of_right_corner_to_middle);

	//if (colorgradient_red<0){colorgradient_red=0.f;}
	//if (colorgradient_green<0){colorgradient_green=0.f;}
	//if (colorgradient_blue<0){colorgradient_blue=0.f;}

	float red_lerpValue = colorgradient_red;
	float green_lerpValue = colorgradient_green;
	float blue_lerpValue = colorgradient_blue;
	vFragColor = color_left * red_lerpValue + color_top * green_lerpValue + color_right * blue_lerpValue;
	vFragColor[3] = 1.f;
	//vFragColor = vec4((red_lerpValue),(green_lerpValue), (blue_lerpValue), 1.f);

} 