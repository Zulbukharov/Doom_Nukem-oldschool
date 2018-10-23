/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaporoz <azaporoz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 16:02:41 by azaporoz          #+#    #+#             */
/*   Updated: 2018/10/23 16:02:41 by azaporoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>
#define SPRITE sprite
#define SPRITE_X (sprite->x - P_X)
#define SPRITE_Y (sprite->y - P_Y)
#define START_X  sprite->draw_start_x
#define START_Y  sprite->draw_start_y
#define END_X sprite->draw_end_x
#define END_Y sprite->draw_end_y
#define TRANSFORM_Y sprite->transform_y
#define SPRITE_W sprite->sprite_width
#define X sprite->x
#define Y sprite->y
#define DIST_X (abs((int)X - (int)P_X))
#define DIST_Y (abs((int)Y - (int)P_Y))

void	restart_enemy(t_wolf *holder, t_sprite *sprite)
{
	holder->frags++;
	SHOOTS = 0;
	sprite->is_sprite = 1;
	sprite->x = sprite->orig_x;
	sprite->y = sprite->orig_y;
	sprite->is_alive = 1;
	sprite->speed = sprite->speed + 0.1;
	sprite->tex_sprite[0] = sprite->s_tex->arr_sprite[0][0];
	sprite->end_frame = 5;
}

void	burning_boss(t_sprite *sprite)
{
	if (IS_SPRITE && !sprite->is_alive && sprite->end_frame >= 5)
	{
		if (sprite->end_frame <= 7)
			CURR_TEX = sprite->s_tex->arr_sprite[1][sprite->end_frame];
		sprite->end_frame++;
	}
	sprite->end_frame = (sprite->end_frame == 8) ? 5 : sprite->end_frame;
	sprite->end_frame = (sprite->end_frame == 9) ? 8 : sprite->end_frame;
}

void	ft_move_boss(t_wolf *holder, t_sprite *sprite)
{
	if (sprite->end_frame >= 9 && DIST_X <= 3 && DIST_Y <= 3)
	{
		get_player_coordinates(holder);
		restart_enemy(holder, sprite);
		holder->hud->lives -= 1;
	}
	if (DIST_X <= 1 && DIST_Y <= 1 && sprite->end_frame < 9 && IS_SPRITE)
	{
		CURR_TEX = sprite->s_tex->arr_sprite[0][sprite->end_frame];
		sprite->end_frame++;
		sprite->is_alive = (sprite->end_frame >= 8) ? 0 : 1;
	}
	if ((sprite->x != P_X || sprite->y != P_Y) &&\
	sprite->is_alive && !holder->pause)
	{
		if ((int)X < (int)P_X && MAP[(int)Y][(int)(X + 0.1)] == '0')
			sprite->x += sprite->speed;
		else if ((int)X > (int)P_X && MAP[(int)Y][(int)(X - 0.1)] == '0')
			sprite->x -= sprite->speed;
		else if ((int)Y > (int)P_Y && MAP[(int)(Y - 0.1)][(int)X] == '0')
			sprite->y -= sprite->speed;
		else if ((int)Y < (int)P_Y && MAP[(int)(Y + 0.1)][(int)X] == '0')
			sprite->y += sprite->speed;
	}
	burning_boss(sprite);
}

void	ft_move_bullet(t_wolf *holder, t_sprite *s)
{
	static float	direction[2] = {0, 0};
	static int		frames = 0;
	static int		gun = 0;

	if (!s->is_alive)
	{
		direction[1] = holder->DIR_X;
		direction[0] = holder->DIR_Y;
		s->x = P_X + direction[1];
		s->y = P_Y + direction[0];
		gun = G;
	}
	else if (s->is_alive && !holder->pause)
	{
		s->x += direction[1];
		s->y += direction[0];
		s->tex_sprite[0] = s->s_tex->arr_sprite[gun][frames++];
	}
	frames = (frames == 3) ? 0 : frames;
	if (MAP[(int)s->y][(int)s->x] != '0' &&\
	s->is_alive && MAP[(int)s->y][(int)s->x] <= '9')
	{
		MAP[(int)s->y][(int)s->x] = MAP[(int)s->y][(int)s->x] + 13;
		s->is_alive = 0;
	}
}