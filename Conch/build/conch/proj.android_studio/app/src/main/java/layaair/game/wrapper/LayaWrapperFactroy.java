package layaair.game.wrapper;

import layaair.game.browser.LayaWrapper;

public class LayaWrapperFactroy {
	public static ILayaLibWrapper createLayaWrapper(){
		return new LayaWrapper();
	}
}
