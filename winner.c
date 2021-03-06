#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "winner.h"
#include "common.h"
#include "game_state.h"
#include "game.h"
#include "main_game.h"
#include "define.h"

/* constants */
#define SELECTOR_NEW_GAME_X   375
#define SELECTOR_NEW_GAME_Y   370
#define NEW_GAME_X            (SELECTOR_NEW_GAME_X + 30)  
#define NEW_GAME_Y            (SELECTOR_NEW_GAME_Y - 6)
#define SELECTOR_EXIT_GAME_X  375
#define SELECTOR_EXIT_GAME_Y  420
#define EXIT_GAME_X           (SELECTOR_EXIT_GAME_X + 30)
#define EXIT_GAME_Y           (SELECTOR_EXIT_GAME_Y - 6)
#define WINNER_MSG_Y          200
#define WINNER_MSG_X(w)       MIDDLE_X(w)
#define USER_SCORE_X          70
#define COMPUTER_SCORE_X      468
#define PLAYER_POINTS_Y       40
#define PLAYER_CARDS_Y        (PLAYER_POINTS_Y + 50)

/* file paths */
#define WINNER_BG_FILE  GFX_DIR "winner.png"
#define SELECTOR_FILE   GFX_DIR "selector.png"
#define GEORGIA_I_FILE  FONTS_DIR "georgiai.ttf"
#define URW_FONT_FILE   FONTS_DIR "urw-bookman-l.ttf"
#define URWB_FONT_FILE  FONTS_DIR "urw-bookman-l-bold.ttf"

/* static data */
static SDL_Texture *winner_tex = NULL;
static SDL_Texture *selector = NULL;
static SDL_Rect selector_pos;
static enum {NEW_GAME, EXIT_GAME} entry;
static bool new_game ;
static enum {USER_WINS, COMPUTER_WINS, DRAWN} game_result;

extern SDL_Renderer *renderer;

void winner_init()
{
	winner_tex = load_image(WINNER_BG_FILE, renderer);
	selector = load_image(SELECTOR_FILE, renderer);

	if (TTF_Init() == -1)
		sdl_ttf_error("Initialisation failed");
	new_game = 0;
	entry = NEW_GAME;
	selector_pos.x = SELECTOR_NEW_GAME_X;
	selector_pos.y = SELECTOR_NEW_GAME_Y;

	if (user_score.points > computer_score.points)
		game_result = USER_WINS;
	else if (user_score.points < computer_score.points)
		game_result = COMPUTER_WINS;
	else if (user_score.gained_cards > computer_score.gained_cards)
		game_result = USER_WINS;
	else if (user_score.gained_cards < computer_score.gained_cards)
		game_result = COMPUTER_WINS;
	else
		game_result = DRAWN;
}

void winner_handle_input()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game_exit(EXIT_SUCCESS);
		break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_UP:
				entry = NEW_GAME;
			break;
                       
			case SDLK_DOWN:
				entry = EXIT_GAME;
			break;

			case SDLK_RETURN:
				if (entry == EXIT_GAME)
					game_exit(EXIT_SUCCESS);
				else
					new_game = 1;
			break;

			default:
			break;
			}
		break;

		default:
		break;
		}
	}
}

void winner_update()
{
	if (new_game) {
		/*remove winner state*/
		top(s).free(); 
		pop(&s);
		
		/*re-init main_game state*/
		top(s).free();
		SDL_Delay(300);
		top(s).init(renderer);
	}

	if (entry == EXIT_GAME) {
		selector_pos.x = SELECTOR_EXIT_GAME_X;
		selector_pos.y = SELECTOR_EXIT_GAME_Y;
	} else {
		selector_pos.x = SELECTOR_NEW_GAME_X;
		selector_pos.y = SELECTOR_NEW_GAME_Y;
	}
}

static void show_winner_msg(SDL_Renderer *renderer)
{
	char buf[27] = "";
	if (game_result == USER_WINS)
		sprintf(buf, "Congratulations, You Win");
	else if (game_result == COMPUTER_WINS)
		sprintf(buf, "Sorry, You Loose");
	else
		sprintf(buf, "Drawn");

	SDL_Texture *tex = set_text_surf(URW_FONT_FILE, 36, buf, 0, 0, 0, renderer);
	renderTexture(tex, renderer, WINNER_MSG_X(tex_w(tex)), WINNER_MSG_Y, NULL);
}

static void show_final_score(SDL_Renderer *renderer)
{
	char buf[20] = "";

	sprintf(buf, "Your points : %2d", user_score.points);
	show_black_text(URW_FONT_FILE, 25, buf, USER_SCORE_X, PLAYER_POINTS_Y, renderer);

	sprintf(buf, "Your cards  : %2d", user_score.gained_cards);
	show_black_text(URW_FONT_FILE, 25, buf, USER_SCORE_X, PLAYER_CARDS_Y, renderer);

	/* we want to get the larger between cards and points
	 * and because cards are more likely to be more than points
	 * we set them first
	 */
	sprintf(buf, "Computer cards  : %2d", computer_score.gained_cards);
	SDL_Texture *cards_tex = set_text_surf(URW_FONT_FILE, 25, buf, 0, 0, 0, renderer);

	renderTexture(cards_tex, renderer, COMPUTER_SCORE_X, PLAYER_CARDS_Y, NULL);
	
	sprintf(buf, "Computer points : %2d", computer_score.points);
	SDL_Texture *points_tex = set_text_surf(URW_FONT_FILE, 25, buf, 0, 0, 0, renderer);

	renderTexture(points_tex, renderer, COMPUTER_SCORE_X, PLAYER_POINTS_Y, NULL);
}

void winner_render(SDL_Renderer *renderer)
{
	renderTexture(winner_tex, renderer, 0, 0, NULL);
	renderTexture(selector, renderer, selector_pos.x, selector_pos.y, NULL);
	
	show_black_text(URWB_FONT_FILE, 25, "New Game", NEW_GAME_X, NEW_GAME_Y, renderer);
	show_black_text(URWB_FONT_FILE, 25, "Exit Game", EXIT_GAME_X, EXIT_GAME_Y, renderer);
	
	show_final_score(renderer);
	show_winner_msg(renderer);

	SDL_RenderPresent(renderer);
}

void winner_free()
{
	TTF_Quit();
	SDL_DestroyTexture(winner_tex);
	SDL_DestroyTexture(selector);
}

game_state_t* set_state_winner()
{
	game_state_t *gs = try_malloc(sizeof(game_state_t));
	gs->init = winner_init;
	gs->handle_input = winner_handle_input;
	gs->update = winner_update;
	gs->render = winner_render;
	gs->free = winner_free;

	return gs;
}
