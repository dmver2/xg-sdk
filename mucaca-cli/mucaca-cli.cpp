/*	$Id: Referenceable.h,v 1.2 2005/09/02 10:01:54 dmitry.vereshchagin Exp $
**
**  Tic-tac-toe SDL UI implementation
**
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include "SDL_syswm.h"
#include <SDL_ttf.h> 
#include <SDL_image.h> 
#include <SDL_timer.h> 

#include "../game_logic/framework.h"
#include "../game_logic/bitset2d.h"

#include <api.h>
#include <network.h>
#include <Referenceable.h>

#include <cstdio>
#include <ctime>
#include <tchar.h>
#include <iostream>
#include <map>
#include <unordered_map>

using namespace GSDK;

#define MATRIX_DIM (a) sizeof(*a) / sizeof(**a)

#define MOVE1 CGameLogic<3>::TTTOutcome::MOVE1
#define MOVE2 CGameLogic<3>::TTTOutcome::MOVE2
#define ONE_WIN CGameLogic<3>::TTTOutcome::ONE_WIN
#define TWO_WIN CGameLogic<3>::TTTOutcome::TWO_WIN
#define DRAW CGameLogic<3>::TTTOutcome::DRAW

#define EMPTY  CGameLogic<3>::TTTField::EMPTY
#define ONE  CGameLogic<3>::TTTField::ONE
#define TWO CGameLogic<3>::TTTField::TWO

#define VARNAME(var) _ ## var
#define EVALUATOR(x)  VARNAME(x)

#define HOST_T(v) \
TCHAR EVALUATOR(v)[0x16] = {0}; \
size_t length = 0; \
host.address_as_text(EVALUATOR(v), length);

#undef main
const float PI = 3.141592653589f;

const TCHAR appname[] = _T("Tic-Tac-Toe");

class CSettings
{
	const TCHAR* server_host;
	const TCHAR* server_port;
	const TCHAR* buf;
	CSettings() : buf(nullptr), server_host(nullptr), server_port(nullptr) {};

public:
	~CSettings() 
	{
		if(buf)
			free(const_cast<TCHAR*>(buf));
	};

	const TCHAR* get_server_host() const
	{
		return server_host;
	};

	const TCHAR* get_server_port() const
	{
		return server_port;
	};

	static CSettings* load(const TCHAR* file_path)
	{
		TCHAR val[0x80] = { 0 };
		CSettings* settings = new CSettings();
		const size_t n = ::GetPrivateProfileString("options", "server", "localhost:2008", val, sizeof(val) / sizeof(val[0]), file_path);
		if (n) {
			settings->buf = _tcsdup(val);
			TCHAR* pch;
			for (pch = const_cast<TCHAR*>(settings->buf); *pch && *pch != ':'; ++pch);
			*pch = '\0';

			settings->server_host = settings->buf;
			settings->server_port = (n != pch - settings->buf) ? pch + 1 : pch;

			return settings;
		}
	}

};

CSettings* g_settings;

class Cell
{
protected:
	const SDL_Point p;
	SDL_Color clr;

public:
	Cell(const SDL_Point& pt) : p(pt), clr({ 0x50, 0x50, 0x50, SDL_ALPHA_OPAQUE })
	{
	}

	virtual ~Cell() = default;

	virtual void Draw(SDL_Renderer* r) = 0;
	virtual void Highlight();
};

class EmptyCell : public Cell
{
public:
	EmptyCell(const SDL_Point& pt) : Cell(pt) {}
	void Draw(SDL_Renderer* r) override;
};

void Cell::Highlight()
{
	clr.a = 0x10;
	clr.r = 0x40;
	clr.g = 0x80;
	clr.b = 0x40;
}

void EmptyCell::Draw(SDL_Renderer* r)
{
}

class OCell : public Cell
{
public:
	OCell(const SDL_Point& pt) : Cell(pt) {}
	void Draw(SDL_Renderer* r) override;
	void Highlight() override;
private:
	void RenderCycle(SDL_Renderer* rend, float radius) const
	{
		const int n = 200;
		const float r = radius;

		SDL_FPoint points[n];
		SDL_FPoint a;
		for (int i = 0; i < n; ++i) {
			float angle = 2 * PI * (i) / n;
			a.x = p.x + r * cos(angle);
			a.y = p.y + r * sin(angle);

			points[i] = a;
		}
		SDL_RenderDrawLinesF(rend, points, n);
	}
};

void OCell::Highlight()
{
	clr.a = 0x10;
	clr.r = 0x80;
	clr.g = 0x40;
	clr.b = 0x40;
}


void OCell::Draw(SDL_Renderer* rend)
{
	SDL_SetRenderDrawColor(rend, clr.r, clr.g, clr.b, clr.a);
	RenderCycle(rend, 100.5);
	RenderCycle(rend, 100);
	RenderCycle(rend, 99.5);
	RenderCycle(rend, 99.);
}

class XCell : public Cell
{
public:
	XCell(const SDL_Point& pt) : Cell(pt) {}
	void Draw(SDL_Renderer* r) override;
};

void XCell::Draw(SDL_Renderer* r)
{
	SDL_SetRenderDrawColor(r, clr.r, clr.g, clr.b, clr.a);
	const SDL_Rect d = { p.x - 100, p.y - 100, p.x + 100, p.y + 100 };
	SDL_RenderDrawLine(r, d.x, d.y, d.w, d.h);
	SDL_RenderDrawLine(r, d.x + 1, d.y, d.w + 1, d.h);
	SDL_RenderDrawLine(r, d.x - 1, d.y, d.w - 1, d.h);
	SDL_RenderDrawLine(r, d.w, d.y, d.x, d.h);
	SDL_RenderDrawLine(r, d.w + 1, d.y, d.x + 1, d.h);
	SDL_RenderDrawLine(r, d.w - 1, d.y, d.x - 1, d.h);
}

template <size_t N>
class IGameSight {
public:
	virtual void Update(const SDL_Point& move, int player) = 0;
	virtual void Display(SDL_Renderer* rend) = 0;
	virtual void Highlight(const bitset2d<N, N>& winline) = 0;

	virtual ~IGameSight() = default;
};

template <size_t M>
class GameView : public IGameSight<M> {
	typedef std::pair<SDL_Texture*, SDL_Rect> tex_rect;
private:
	Cell* matrix[M][M];
	string message;
	std::unordered_map<string, tex_rect> texcache;
	TTF_Font* font;

private:
	void InitMatrix();

public:
	GameView();
	virtual void Update(const SDL_Point& move, int player) override;
	virtual void Display(SDL_Renderer* rend) override;
	virtual void Highlight(const bitset2d<M, M>& winline) override;
	void DisplayMessage(SDL_Renderer* rend);
	virtual ~GameView();

	void PostMessage(const string& message)
	{
		this->message = message;
	}

	const string& Message() const
	{
		return this->message;
	}


	void Clear() {
		this->InitMatrix();
	}
};

template <size_t M>
void GameView<M>::InitMatrix()
{
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			matrix[i][j] = new EmptyCell({ 166 + i * 333, 166 + j * 333 });
		}
	}
}

template<size_t M>
GameView<M>::GameView()
{
	//	const TCHAR fontPath[] = "..\\mucaca-cli\\resources\\TwemojiMozilla.ttf";
	const TCHAR fontPath[] = "..\\mucaca-cli\\resources\\TEMPSITC.TTF";

	this->font = TTF_OpenFont(fontPath, 24);
	if (font == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Load image %s ERROR: %s", fontPath, TTF_GetError());
		exit(EXIT_FAILURE);
	}
	InitMatrix();
}

template <size_t M>
void GameView<M>::Update(const SDL_Point& move, int player)
{
	Cell*& cell = matrix[move.y][move.x];
	if (cell)
		delete cell;
	cell = (ONE == player) ?
		(Cell*) new XCell({ 166 + move.x * 333, 166 + move.y * 333 })
		: (Cell*) new OCell({ 166 + move.x * 333, 166 + move.y * 333 });
}

template <size_t M>
void GameView<M>::Display(SDL_Renderer* rend)
{
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			if (matrix[i][j])
			{
				matrix[i][j]->Draw(rend);
			}
		}
	}
}

template <size_t M>
void GameView<M>::DisplayMessage(SDL_Renderer* rend)
{
	if (this->message.length()) {
		SDL_Texture* texture;
		SDL_Rect rect1;
		std::unordered_map<string, tex_rect>::const_iterator found = this->texcache.find(message);
		if (found != texcache.end()) {
			texture = found->second.first;
			rect1 = found->second.second;
		}
		else {
			::PrepareTextMessage(rend, this->message.data(), font, &texture, &rect1);
			texcache[message] = tex_rect(texture, rect1);
		}
		SDL_RenderCopy(rend, texture, NULL, &rect1);
	}
}


template<size_t M>
void GameView<M>::Highlight(const bitset2d<M, M>& winline)
{
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < M; ++j) {
			if (winline(i, j)) {
				matrix[i][j]->Highlight();
			}
		}
	}
}

template<size_t M>
GameView<M>::~GameView()
{
	SDL_free(font);

	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < M; ++j) {
			if (matrix[i][j]) {
				delete matrix[i][j];
			}
		}
	}

	for (std::unordered_map<string, tex_rect>::const_iterator it = texcache.begin(); it != texcache.end(); ++it)
	{
		SDL_DestroyTexture( it->second.first );
	}
}


template <int N>
class CGameLogicProxy : public CGameLogic<N> {
public:
	GSDK::Signal_v1<const SDL_Point&> sig_reply;
	CGameLogicProxy()
	{};

	int MoveRpc(const SDL_Point& x, CGameLogic<N>::TTTField p) {
		if (this->Move(x, p)) {
			const SDL_Point r = this->Countermove();
			sig_reply.call(r);
		}
		else {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Proxy error");
			return -1;
		}
		return 0;
	}
};

template <int N>
class CNetLogicProxy {
	typedef CGameLogic<N> logic;
private:
	GSDK::NetInterface* m_net;
	GSDK::SigSlot m_slot_connected;
	GSDK::SigSlot m_slot_reconnected;
	GSDK::SigSlot m_slot_start;
	GSDK::SigSlot m_slot_turn;
	GSDK::SigSlot m_slot_quit;
	GSDK::SigSlot m_slot_disconnect;
	GSDK::Host m_server;
	volatile typename logic::TTTOutcome outcome;
	volatile typename logic::TTTField player;
	SDL_Point deferred_move;

public:
	GSDK::Signal_v1<const SDL_Point&> sig_reply;

	CNetLogicProxy() : m_net(new NetInterface()), outcome(MOVE2), player(ONE), deferred_move(logic::NOVIABLEPATH) {
		m_net->set_debug(true);
		m_slot_connected = m_net->sig_host_connected().connect(this, &CNetLogicProxy::host_attached).set_persistent(true);
		m_slot_reconnected = m_net->sig_host_reconnected().connect(this, &CNetLogicProxy::host_reconnected).set_persistent(true);
		m_slot_disconnect = m_net->sig_host_disconnected().connect(this, &CNetLogicProxy::host_disconnected).set_persistent(true);
		m_slot_start = m_net->sig_packet_received(_T("start")).connect(this, &CNetLogicProxy::on_start).set_persistent(true);
		m_slot_turn = m_net->sig_packet_received(_T("turn")).connect(this, &CNetLogicProxy::on_turn).set_persistent(true);
		m_slot_quit = m_net->sig_packet_received(_T("quit")).connect(this, &CNetLogicProxy::on_quit).set_persistent(true);
		m_net->start_client("0");// any free port

		connect();
	}

	GSDK::Host& connect()
	{
		//	m_server = m_net->connect(IPAddress("127.0.0.1", 2008));
		return m_server = m_net->connect(IPAddress(g_settings->get_server_host(), g_settings->get_server_port()));
	}

	~CNetLogicProxy() 
	{
		//quit
//		m_server = nullptr; //release_ref
		m_net->stop();
		delete m_net;
	}

	int MoveRpc(const SDL_Point& move, typename CGameLogic<N>::TTTField p) {
		if (m_server.is_disconnected()) {
			deferred_move = move;
			m_server = this->connect();
			outcome = MOVE1;
			return 1;
		} 
		else 
			return do_move_rpc(p, move);
	}

	
	int do_move_rpc(typename CGameLogic<N>::TTTField& p, const SDL_Point& move)
	{
		Packet movepack;
		movepack.output << (int)p << move.x << move.y;
		m_server.send("turn", movepack);
		outcome = MOVE2;

		return 0;
	}

	void host_attached(const GSDK::Host& host)
	{
		HOST_T (addr);
		SDL_Log("attached: %s", _addr);
		start();
	}

	void host_reconnected(const GSDK::Host& host)
	{
		HOST_T (addr);
		SDL_Log("host_reconnected: %s", _addr);
		start();  // expected broken server state
		// otherwise if restored session persist try to use deferred_move
		// if(deferrd_move != logic::NOVIABLEPATH) {
		//		MoveRpc(deferrd_move, player);
		//		deferred_move == logic::NOVIABLEPATH;
		// }
	}


	void host_disconnected(const GSDK::Host& host)
	{
//		SDL_Log("disconnected:");
		HOST_T (addr);
		SDL_Log("disconnected: %s %d", _addr, m_server.is_disconnected());
	}

	void start()
	{
		SDL_Log("start...");
		GSDK::Packet startpack;
		startpack.output << appname << 0 << 1 << player;
		m_server.send("start", startpack);
	}

	void on_start(GSDK::Packet& p, const GSDK::Host& h)
	{
		SDL_Log("on start");
		outcome = MOVE1;
	}


	void on_turn(Packet& p, const Host& h)
	{
		SDL_Log("on turn");
		int rc;
		SDL_Point reply;
		p.input >> rc >> reply.x >> reply.y; // >> player;
		outcome = MOVE1;
		sig_reply.call(reply);
	}

	void on_quit(Packet& packt, const Host& host)
	{
		TCHAR buf[0x16];
		size_t length = 0;

		SDL_Log("quit host: %s", host.address_as_text(buf, length));
	}

	void Clear()
	{
		Packet quitpack;
		m_server.send("quit", quitpack);
		// swap player
		player = logic::TTTField(TWO + 1 - player);
	}

	typename logic::TTTOutcome Outcome() const
	{
		return outcome;
	}

};


template <int N, class Vw, class Gl>
class CGameShell
{
private:
	CGameLogic<N> local_game;
	Gl proxy;
	GSDK::SigSlot reply;
	Vw view;

public:
	Vw& View() { return view; };

	CGameShell()
	{
		reply = proxy.sig_reply.connect(this, &CGameShell<N, Vw, Gl>::OnMoveResponse);
		//		reply.set_persistent(true);
	};

	//CGameShell(Vw& view1, Gl& proxy1) : view(view1), proxy(proxy1)
	//{
	//	this->CGameShell();
	//};

	void Move(const SDL_Point& move) {
		if (proxy.Outcome() == MOVE1)
		{
			if (local_game.Move(move, ONE)) // you've moved & hit free cell
			{
				int rc = proxy.MoveRpc(move, ONE);
				if (0 == rc)
				{
					view.PostMessage("waiting ...");
				}
				else
				{
					view.PostMessage("error negotiating game logic");
				}
			}
			else
			{
				view.PostMessage("invalid move");
			}
		}
		else
		{
			view.PostMessage("..."); //"proxy not ready"
		}
	}

	void OnMoveResponse(const SDL_Point& reply) {
		const SDL_Point m = local_game.Lastmove();
		view.PostMessage("");
		view.Update(m, MOVE1);

		//				CGameLogic<3>::TTTOutcome outcome = ;
		bitset2d<3, 3> winline;
		if (MOVE2 < local_game.CheckWin(winline, ONE)) {
			view.Highlight(winline);
			winline.reset();
		}
		else if (CGameLogic<N>::NOVIABLEPATH != reply) {
			CounterMove(reply);
			//			view.SetMessage("error receiving reply");
		}
	}


	int CounterMove(const SDL_Point& move) {
		if (local_game.Move(move, TWO))
		{
			view.Update(move, MOVE2);

			bitset2d<3, 3> winline;
			//			CGameLogic<3>::TTTOutcome outcome = ;
			if (MOVE2 < local_game.CheckWin(winline, TWO)) {
				view.Highlight(winline);
			}
			winline.reset();
		}
		return 0;
	}

	void Clear()
	{
		local_game.Clear();
		proxy.Clear();
		view.Clear();
	}

	typename CGameLogic<N>::TTTOutcome Outcome() const
	{
		return local_game.Outcome();
	}

};

class LogFunctor
{
public:
	void operator() (const TCHAR* topic, int severity, const TCHAR* msg)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LogPriority(SDL_LOG_PRIORITY_DEBUG + severity), msg);
	}
};

void DrawCursor(SDL_Renderer* rend, SDL_Rect c)
{
	SDL_SetRenderDrawColor(rend, 0x40, 0x40, 0x40, SDL_ALPHA_OPAQUE);
	const SDL_Rect d = { c.x * 333, c.y * 333, c.w * 333, c.h * 333 };
	SDL_RenderDrawRect(rend, &d);
	const SDL_Rect d1 = { d.x + 1, d.y + 1, d.w - 2, d.h - 2 };
	SDL_RenderDrawRect(rend, &d1);
}

void PrepareTextMessage(SDL_Renderer* renderer, const TCHAR* text,
	TTF_Font* font, SDL_Texture** texture, SDL_Rect* rect) {

	int text_width;
	int text_height;
	SDL_Surface* surface;
	SDL_Color textColor = { 255, 255, 255, 0 };

	surface = TTF_RenderText_Solid(font, text, textColor);
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);
	rect->x = (1000 - text_width) / 2;
	rect->y = (1000 - text_height) / 2;
	rect->w = text_width;
	rect->h = text_height;
}

#if defined(WIN32) && defined(GCL_HICON)
void SetWindowIcon(SDL_Window* sdlWindow) {
	HINSTANCE handle = ::GetModuleHandle(nullptr);
	HICON icon = ::LoadIcon(handle, "IDI_MAIN_ICON");
	if (icon != nullptr) {
		SDL_SysWMinfo wminfo;
		SDL_VERSION(&wminfo.version);
		if (SDL_GetWindowWMInfo(sdlWindow, &wminfo) == 1) {
			HWND hwnd = wminfo.info.win.window;
			::SetClassLong(hwnd, GCL_HICON, reinterpret_cast<LONG>(icon));
		}
	}
}
#endif

int _tmain(int argc, const TCHAR* argv[])
{
	::srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window* win = SDL_CreateWindow(appname,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1000, 1000, 0);

	SetWindowIcon(win);

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	SDL_Surface* surface;

	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
	const TCHAR imagePath[] = "..\\mucaca-cli\\resources\\cracked-texture.png";
	surface = IMG_Load(imagePath);
	if (!surface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Load image %s ERROR: %s", imagePath, IMG_GetError());
	}
	else {
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Load image %s completed successfully", imagePath);
	}

	TTF_Init();

	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

	SDL_FreeSurface(surface);

	SDL_Rect dest;

	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	int close = 0;

	const TCHAR* msg = 0;
	SDL_Rect cursor{ 0, 0, 1, 1 };
	cursor.x = 1;
	cursor.y = 1;

	const SigSlot logSlot2 = Log::connect_functor(LogFunctor());
	Network_Setup::startup();
	{
		g_settings = CSettings::load(string(".\\").append(appname).append(".ini").data());
		CGameShell<3, GameView<3>, CNetLogicProxy<3> > gshell;

		int state = 0;

		while (!close)
		{
			SDL_Event e;
			bool arrow_pressed = false;
			bool turn = false;
			while (SDL_PollEvent(&e))
			{
				const SDL_Event* event = &e;
				switch (event->type) {

				case SDL_MOUSEMOTION:
					cursor.x = event->button.x / 333;
					cursor.y = event->button.y / 333;
					break;

				case SDL_MOUSEBUTTONUP:
					cursor.x = event->button.x / 333;
					cursor.y = event->button.y / 333;
					if (state == 0) {
						turn = event->button.clicks == 2;
					}
					break;

				case SDL_QUIT:
					close = 1;
					break;

				case SDL_KEYDOWN:
					switch (event->key.keysym.scancode)
					{
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_UP:
						--cursor.y;
						arrow_pressed = 1;
						break;
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_LEFT:
						--cursor.x;
						arrow_pressed = 1;
						break;
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_DOWN:
						++cursor.y;
						arrow_pressed = 1;
						break;
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_RIGHT:
						arrow_pressed = 1;
						++cursor.x;
						break;
					case SDL_SCANCODE_SPACE:
					case SDL_SCANCODE_RETURN:
						if (state == 1) {
							turn = true;
						}
						break;
					}
				}
			}

			if (cursor.x > 2)
				cursor.x = 2;

			if (cursor.x < 0)
				cursor.x = 0;

			if (cursor.y > 2)
				cursor.y = 2;

			if (cursor.y < 0)
				cursor.y = 0;

			if (turn)
			{
				const SDL_Point move{ cursor.x, cursor.y };
				gshell.Move(move);
			} // turn

			SDL_RenderClear(rend);
			for (dest.y = 0; dest.y < 1000; dest.y += dest.h)
			{
				for (dest.x = 0; dest.x < 1000; dest.x += dest.w)
				{
					SDL_RenderCopy(rend, tex, NULL, &dest);
				}
			}

			gshell.View().Display(rend);

			//if (arrow_pressed)
			//{
			//	SDL_Log("{%d, %d}", cursor.x, cursor.y);
			//}
			DrawCursor(rend, cursor);

			gshell.View().DisplayMessage(rend);

			//END PAINT
			SDL_RenderPresent(rend);

			bool clear;
			switch (gshell.Outcome())
			{
			case ONE_WIN:
				clear = true;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
					"CONGRATULATIONS",
					"YOU WIN",
					win);
				break;

			case TWO_WIN:
				clear = true;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"END OF GAME",
					"YOU LOOSE",
					win);
				break;

			case DRAW:
				clear = true;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
					"END OF GAME",
					"DRAW",
					win);
				break;
			default:
				clear = false;
			}

			if (clear)
			{
				gshell.Clear();
			}

			SDL_Delay(1000 / 30);
		} // while(!close)
		delete g_settings;
	} // network
	Network_Setup::shutdown();

	SDL_DestroyTexture(tex);

	SDL_DestroyRenderer(rend);

	SDL_DestroyWindow(win);
	return 0;
}
