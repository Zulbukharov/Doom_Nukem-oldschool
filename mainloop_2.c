/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainloop_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaporoz <azaporoz@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 16:06:09 by azaporoz          #+#    #+#             */
/*   Updated: 2018/10/23 16:06:09 by azaporoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

void		ft_check_pause(t_wolf *holder, const Uint8 *keystate)
{
	if (keystate[SDL_SCANCODE_P] && !holder->starting)
	{
		SDL_WarpMouseInWindow(WINDOW, WIDTH / 2, HEIGHT / 2);
		draw_text(holder, "Pause", WIDTH / 2, 10);
		SDL_RenderPresent(holder->renderer);
		holder->pause = (holder->pause == -1) ? 0 : 1;
		if (Mix_PausedMusic() == 0 && holder->pause)
			Mix_PauseMusic();
		else if (!holder->pause)
			Mix_ResumeMusic();
		if (SDL_GetRelativeMouseMode() == SDL_TRUE)
			SDL_SetRelativeMouseMode(SDL_FALSE);
		else
			SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else if (holder->hud->lives <= 3)
	{
		draw_text(holder, "RETRY? y/n", WIDTH / 2, HEIGHT / 2 - 200);
		holder->retry_state = 1;
		SDL_RenderPresent(holder->renderer);
		holder->pause = (holder->pause == -1) ? 0 : 1;
	}
}

void		ft_check_other(t_wolf *holder, const Uint8 *keystate)
{
	if (keystate[SDL_SCANCODE_DOWN] && holder->starting)
	{
		SDL_RenderCopy(holder->renderer, START_SCR, NULL, NULL);
		draw_text(holder, "	  Play", WIDTH / 2 - 10, HEIGHT / 2);
		draw_text(holder, " > Quit", WIDTH / 2 - 10, HEIGHT / 2 + 15);
		draw_text(holder, "PRESS ENTER TO SELECT", WIDTH / 2, HEIGHT - 25);
		holder->start->play_select = 0;
		SDL_RenderPresent(holder->renderer);
	}
	if (keystate[SDL_SCANCODE_UP] && holder->starting)
	{
		SDL_RenderCopy(holder->renderer, START_SCR, NULL, NULL);
		draw_text(holder, " > Play", WIDTH / 2 - 10, HEIGHT / 2);
		draw_text(holder, "	  Quit", WIDTH / 2 - 10, HEIGHT / 2 + 15);
		draw_text(holder, "PRESS ENTER TO SELECT", WIDTH / 2, HEIGHT - 25);
		holder->start->play_select = 1;
		SDL_RenderPresent(holder->renderer);
	}
}

void		check_button(t_wolf *holder, t_camera *camera)
{
	if (CHECK_UP_X_PLUS2 == ':' && holder->keys.enter)
	{
		holder->shadows = 0;
		CHECK_UP_X_PLUS2 = ';';
	}
	else if (CHECK_UP_X_PLUS2 == ';' && holder->keys.enter)
	{
		holder->shadows = 1;
		CHECK_UP_X_PLUS2 = ':';
	}
	else if (CHECK_UP_X_MINUS2 == ':' && holder->keys.enter)
	{
		holder->shadows = 0;
		CHECK_UP_X_MINUS2 = ';';
	}
	else if (CHECK_UP_X_MINUS2 == ';' && holder->keys.enter)
	{
		holder->shadows = 1;
		CHECK_UP_X_MINUS2 = ':';
	}
}