/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolgin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/15 18:44:23 by avolgin           #+#    #+#             */
/*   Updated: 2018/09/28 19:59:18 by avolgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

unsigned int	get_pixel(SDL_Surface *surface, int x, int y)
{
	unsigned int	*pixels;

	pixels = (unsigned int*)surface->pixels;
	return (pixels[(y * surface->w) + x]);
}

/*
** To get 60 fps
**	if (holder->frame_time > holder->frame_delay)
**	{
**		SDL_Delay(holder->frame_time - holder->frame_delay);
**		fps = ft_strdup("60");
**	}
**	else
*/

void			draw_fps(t_wolf *holder, TTF_Font *font)
{
	char		*fps;
	char		*tmp;
	SDL_Surface	*surf;
	SDL_Texture	*texture;

	holder->frame_time = SDL_GetTicks() - holder->frame_start;
	if (holder->frame_time < holder->frame_delay)
	{
		SDL_Delay(holder->frame_delay - holder->frame_time);
		fps = ft_itoa(60);
	}
	else
		fps = ft_itoa(1000 / holder->frame_time);
	tmp = fps;
	fps = ft_strjoin("FPS: ", fps);
	free(tmp);
	surf = TTF_RenderText_Solid(font, fps, WHITE);
	free(fps);
	texture = SDL_CreateTextureFromSurface(holder->renderer, surf);
	SDL_RenderCopy(holder->renderer, texture, NULL, &(SDL_Rect){5, 5, 56, 14});
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);
	draw_score(holder);
}

void			draw_text(t_wolf *holder, char *text, int x, int y)
{
	SDL_Surface	*surf;
	SDL_Texture	*texture;
	SDL_Rect	dest;

	surf = TTF_RenderText_Solid(holder->camera->font, text, WHITE);
	texture = SDL_CreateTextureFromSurface(holder->renderer, surf);
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	dest.x = x - (dest.w >> 1);
	dest.y = y;
	SDL_RenderCopy(holder->renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);
}

unsigned int alter_color_fixed(unsigned int color, int coefficient)
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int ret;

//printf("coefficient = %d\n", coefficient);
	if (coefficient > 256)
		return (0xff000000);
	r = INT_F((color >> 16) & 0xff);
	g = INT_F((color >> 8) & 0xff);
	b = INT_F(color & 0xff);
//	printf("r before == %d\n", r);
//	printf("coefficient == %f\n", coefficient);
	r = r - (FIX_MULT(r, coefficient));
	g = g - (FIX_MULT(g, coefficient));
	b = b - (FIX_MULT(b, coefficient));
//	printf("r after == %d\n", r);
//	g = g - (g * coefficient);
//	b = b - (b * coefficient);
	r = FIXED_I(r);
	g = FIXED_I(g);
	b = FIXED_I(b);
	ret = r;
	ret <<= 8;
	ret = ret | g;
	ret <<= 8;
	ret = ret | b;
	return (ret | 0xFF000000);
}



unsigned int alter_color(unsigned int color, float coefficient)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned int ret;

//printf("coefficient = %d\n", coefficient);
if (coefficient > 1)
	return (0xff000000);
	r = color >> 16;
	g = color >> 8;
	b = color;
	r = r - (r * coefficient);
	g = g - (g * coefficient);
	b = b - (b * coefficient);
	ret = r;
	ret <<= 8;
	ret = ret | g;
	ret <<= 8;
	ret = ret | b;
	return (ret | 0xFF000000);
}

void			draw_floor_simple(t_wolf *holder, \
unsigned int **buffer, unsigned int x)
{
	int		i;
	float	current_dist;
	float	weight;
	float	current_floor_x;
	float	current_floor_y;

	i = holder->camera->draw_end;
	while (++i < holder->height)
	{
		current_dist = FIXED_F(FIX_DIV(INT_F(holder->height), \
INT_F((i << 1) - holder->height - 2 * (holder->updown + holder->extra_updown))));
		weight = current_dist / PERP_DIST_WALL;
		current_floor_x = weight * WALL_X_FL + (1.0 - weight) * P_X;
		current_floor_y = weight * WALL_Y_FL + (1.0 - weight) * P_Y;
		buffer[i][x] = get_pixel(holder->camera->texture[4], \
							FLOOR_TEX_X >> 1, FLOOR_TEX_Y);
		if (holder->shadows) //Faster! 40 vs 45 fps. But not always?
			buffer[i][x] = alter_color_fixed(buffer[i][x], (int)((float)current_dist * 256) / 10);
	}
}

// void			draw_floor_simple(t_wolf *holder, \
// unsigned int buffer[][holder->width], unsigned int x)
// {
// 	int		i;
// 	float	current_dist;
// 	float	weight;
// 	float	current_floor_x;
// 	float	current_floor_y;

// 	i = holder->camera->draw_end;
// 	holder->state = 0;
// //	printf("draw_start = %d, draw_end = %d\n", holder->camera->draw_start, holder->camera->draw_end);
// 	while (++i < holder->camera->draw_end)
// 	{
		
// //		current_dist = ((float)HEIGHT) / (2 * i - HEIGHT - 2 * (holder->updown + holder->extra_updown));
// 		current_dist = FIXED_F(FIX_DIV(INT_F(holder->height),	\
// INT_F((i << 1) - holder->height - 2 * (holder->updown + holder->extra_updown))));
// 		weight = current_dist / PERP_DIST_WALL;
// 		current_floor_x = weight * WALL_X_FL + (1.0 - weight) * P_X;
// 		current_floor_x = (fabsf(current_floor_x) < 1) ? 1 : current_floor_x;
// 		current_floor_y = weight * WALL_Y_FL + (1.0 - weight) * P_Y;
// 		current_floor_y = (current_floor_y < 0) ? 0 : current_floor_y;
// //		old_wall_height = new_wall_height;
// //		new_wall_height = holder->height_map[(int)current_floor_y][(int)current_floor_x - 1];
// //		if (old_wall_height != new_wall_height)
// //			holder->state = !holder->state;
// //		if (new_wall_height >= holder->current_height && !holder->state)
// 		buffer[i][x] = get_pixel(holder->camera->texture[4], \
// 							FLOOR_TEX_X >> 1, FLOOR_TEX_Y);
// //		else
// //		{
// //			holder->state = !holder->state;
// //			buffer[i][x] = -1677721;
// //		}
// 		if (holder->shadows) //Faster! 40 vs 45 fps. But not always?
// 			buffer[i][x] = alter_color_fixed(buffer[i][x], (int)((float)current_dist * 256) / 10);
// //		if (holder->shadows)
// //			buffer[i][x] = alter_color(buffer[i][x], current_dist / 10);
// 	}
// }


void			draw_floor1(t_wolf *holder, \
unsigned int buffer[holder->height][holder->width], unsigned int x)
{
	int		i;
	float	current_dist;
	float	weight;
	float	current_floor_x;
	float	current_floor_y;

float old_wall_height;
float new_wall_height = holder->wall_height;
//	i = holder->camera->draw_end;
	holder->state = 0;
//	printf("draw_start = %d, draw_end = %d\n", holder->camera->draw_start, holder->camera->draw_end);
	for (int i = holder->height - 1; i > holder->camera->draw_end; i--)
	{
//		current_dist = ((float)HEIGHT) / (2 * i - HEIGHT - 2 * (holder->updown + holder->extra_updown));
		current_dist = FIXED_F(FIX_DIV(INT_F(holder->height),	\
INT_F((i << 1) - holder->height - 2 * (holder->updown + holder->extra_updown - holder->wall_height))));
		weight = current_dist / PERP_DIST_WALL;
		current_floor_x = weight * WALL_X_FL + (1.0 - weight) * P_X;
		current_floor_x = (fabsf(current_floor_x) < 1) ? 1 : current_floor_x;
		current_floor_y = weight * WALL_Y_FL + (1.0 - weight) * P_Y;
		if (current_floor_x <= 0 || current_floor_y < 0)
			continue;
//		current_floor_y = (current_floor_y < 0) ? 0 : current_floor_y;
		holder->wall_height = (holder->height_map[(int)current_floor_y - 1][(int)current_floor_x - 1] > 300) ? 300 / current_dist \
		: holder->height_map[(int)current_floor_y - 1][(int)current_floor_x - 1] / current_dist;
		old_wall_height = new_wall_height;
		new_wall_height = holder->height_map[(int)current_floor_y - 1][(int)current_floor_x - 1];
		if (old_wall_height != new_wall_height)
			holder->state = !holder->state;
		if (!holder->state)
		{
		buffer[i][x] = get_pixel(holder->camera->texture[4], \
							FLOOR_TEX_X >> 1, FLOOR_TEX_Y);
//		buffer[HEIGHT - i][x] = get_pixel(holder->camera->texture[4], \
//							FLOOR_TEX_X >> 1, FLOOR_TEX_Y);
		}
		else
		{
			holder->state = !holder->state;
			buffer[i][x] = -1677721;
		}
		if (holder->shadows) //Faster! 40 vs 45 fps. But not always?
			buffer[i][x] = alter_color_fixed(buffer[i][x], (int)((float)current_dist * 256) / 10);
//		if (holder->shadows)
//			buffer[i][x] = alter_color(buffer[i][x], current_dist / 10);
	}
}

void			draw_floor(t_wolf *holder, t_camera *camera, \
					unsigned int buffer[holder->height][holder->width], unsigned int x)
{
	if (camera->side == 0 && camera->ray_dir_x > 0)
	{
		camera->wall_x_f = camera->map_x;
		camera->wall_y_f = camera->map_y + camera->wall_x;
	}
	else if (camera->side == 0 && camera->ray_dir_x < 0)
	{
		camera->wall_x_f = camera->map_x + 1.0;
		camera->wall_y_f = camera->map_y + camera->wall_x;
	}
	else if (camera->side == 1 && camera->ray_dir_y > 0)
	{
		camera->wall_x_f = camera->map_x + camera->wall_x;
		camera->wall_y_f = camera->map_y;
	}
	else
	{
		camera->wall_x_f = camera->map_x + camera->wall_x;
		camera->wall_y_f = camera->map_y + 1.0;
	}
	if (camera->draw_end < 0)
		camera->draw_end = holder->height;
 	draw_floor1(holder, buffer, x);
}
