/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_screen_3_nps.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaporoz <azaporoz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 13:22:50 by azaporoz          #+#    #+#             */
/*   Updated: 2018/10/23 13:22:50 by azaporoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

t_sprite	*start_enemy(t_wolf *holder, int num)
{
	t_sprite *sprite;

	sprite = (t_sprite*)malloc(sizeof(t_sprite));
	holder->sprite[num] = sprite;
	sprite->shoots = 0;
	sprite->speed = 0.1;
	sprite->is_alive = 0;
	sprite->sprite_found = 0;
	sprite->is_sprite = 0;
	sprite->end_frame = 0;
	return (sprite);
}

void		start_arcade_sprite(t_wolf *holder, int num)
{
	t_sprite	*sprite;

	sprite = start_enemy(holder, num);
	sprite->is_sprite = get_sprite_coordinates(holder, 'A', num);
	sprite->end_frame = 5;
	sprite->s_tex = holder->sprite_tex[0];
	holder->sprite[num] = sprite;
	sprite->tex_sprite[0] = holder->sprite_tex[0]->arr_sprite[0][0];
}

void		start_bullet_sprite(t_wolf *holder, int num)
{
	t_sprite	*bullet;

	bullet = start_enemy(holder, num);
	bullet->s_tex = holder->sprite_tex[1];
	bullet->x = P_X + holder->DIR_X;
	bullet->y = P_Y + holder->DIR_Y;
	bullet->orig_x = bullet->x;
	bullet->orig_y = bullet->y;
	bullet->speed = 0.5;
	bullet->sprite_found = 1;
	bullet->tex_sprite[0] = holder->sprite_tex[1]->arr_sprite[0][0];
}

void		start_car_sprite(t_wolf *holder, int num)
{
	t_sprite	*car;

	car = start_enemy(holder, num);
	car->s_tex = holder->sprite_tex[2];
	car->is_sprite = get_sprite_coordinates(holder, 'C', num);
	car->texture = 99;
	car->tex_sprite[0] = holder->sprite_tex[2]->arr_sprite[0][0];
}
