// <script src="https://gist.github.com/jamescurran/9766860.js"></script>

///<reference path="eventsource.ts" />

class TestObj extends noveltheory.EventSource
{
	constructor()
	{
		super();
		this.SetName("TestObj");
		this.AddAllowedEvent("ding");
	}

	ring()
	{
		this.trigger("ding", {time: new Date()}); 
	}

	private static _instance: TestObj;
	public static Instance(): TestObj
	{
		if (TestObj._instance == null)
			TestObj._instance = new TestObj();
		return TestObj._instance;
	}
}

function DoIt()
{
    alert("before");
    TestObj.Instance().on("ding", this, (e) => { alert("ding @" + e.data.time); });
    TestObj.Instance().ring();   // shows alert
    TestObj.Instance().off("ding");
    TestObj.Instance().ring();	// does not show alert.
    alert("done");
}
