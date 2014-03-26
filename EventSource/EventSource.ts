
module noveltheory 
{
/*
using a class which uses noveltheory.EventSource.

Syntax:
    objSomeObj.on("event", context, handlerFunction, [optional data])

Example:
	status.on("changed", this, function(ev) { alert ("changed");} );

	event - string, name of the subscribed event.  
	context - will the the "this" object when the handler function is called.  Most like you'd want it to be the "this" property of the calling class.
	handleFunction - function called when event is triggered.  An EventArgs object (described below) is passed as it's argument
	data - an optional object which is included with in the EventArgs when the event is triggered.

EventArgs
	source - the object triggering the event.
	type  - string, the name of the event triggered
	data - extra data (from above), might include more properties from the triggering object.

----
Syntax:
    objSomeObj.one("event", context, handlerFunction, [optional data])

Example:
	status.one("changed", this, function(ev) { alert ("changed");} );

	paramters are as above.

	handlerFunction is called only once, on the next time the event is triggers.

----
Syntax:
    objSomeObj.off("event", [context], [handlerFunction])

Example:
	// remove all "changed" handlers.
	status.off("changed");   

	// remove all "changed" handlers for this context.
	status.off("changed", this );

	// remove the onStatusChanged handlers for this context.
	// Note: this cannot be an inline function.
	status.off("changed", this, onStatusChanged);

	

*/




	class EventHandlerItem
	{
		constructor(public type : string,
			public data: any,
			public context:any,
			public repeat: boolean,
			public action : (args: EventArgs)=>void)
					{
					}
	}

	export class EventArgs
	{
		constructor(
			public source: EventSource,
			public type : string,
			public data : any)
			{
			}
	}

	export class EventSource
	{
		private events: EventHandlerItem[] = [];
		private allowedEvents: string[] = [];
		private name: string = "";

		/* protected */ SetName(name: string) : void
		{
			if (name.lastIndexOf(".") != name.length-1)
				name = name+"."
			this.name = name;
		}

		/* protected */ AddAllowedEvent(type:string)
		{
			type = this.PrefixName(type);
			this.allowedEvents.push(type);
		}

		private PrefixName(type:string)
		{
			if (this.startsWith(type, this.name))
				return type;
			else
				return this.name + type;
		}

		private startsWith(longerString:string, shorterString:string) :boolean
		{
			return longerString.length >= shorterString.length && shorterString == longerString.substr(0, shorterString.length);
		}

		on(type: string, context: any, handler: (args: EventArgs) => void, data?: any, repeat?:boolean): EventSource
		{
			if (typeof(repeat) === "undefined")
				repeat = true;

			var allowed = true;
			type = this.PrefixName(type);
			if (this.allowedEvents.length > 0)
			{
				allowed = false;
				this.allowedEvents.forEach((ev) =>
					{
						if (ev === type) allowed = true;
					});
			}

			if (!allowed)
				throw new RangeError("Error:'"+type+"' is not a known event for this object");

			var hdlr = new EventHandlerItem(type, data  || {}, context, repeat, handler);
			this.events.push(hdlr);
			return this;
		}

		one(type: string, context: any, handler: (args: EventArgs) => void, data?: any)
		{
			return this.on(type, context, handler, data, false);
		}

		off(type: string, context?: any, handler?: (args: EventArgs) => void): EventSource
		{
			var newhdlrs : EventHandlerItem[] = [];

			type = this.PrefixName(type);
			if (typeof(handler)!=="undefined")
				this.events.forEach(ehi=> { if (ehi.type !== type || ehi.context !== context || ehi.action !== handler) newhdlrs.push(ehi); });
			else if (typeof (context) !== "undefined")
				this.events.forEach(ehi=> { if (ehi.type !== type || ehi.context !== context) newhdlrs.push(ehi); });
			else
				this.events.forEach(ehi=> { if (ehi.type !== type) newhdlrs.push(ehi); });

			this.events = newhdlrs;
			return this;
		}

		trigger(type: string, extraData?: any)
		{
			var newhdlrs : EventHandlerItem[] = [];
		
			type = this.PrefixName(type);
			this.events.forEach(ehi=>
			{
				if (ehi.type === type)
				{
					var data = ehi.data || {};
					this.extend(data, extraData || {});
					ehi.action.apply(ehi.context, [new EventArgs(this, ehi.type, data)]);
					if (ehi.repeat)
						newhdlrs.push(ehi);
				}
				else
					newhdlrs.push(ehi);

			});

			this.events = newhdlrs;

        }

        private extend(obj1: any, obj2: any): any
        {
            for (var attrname in obj2)
            {
                obj1[attrname] = obj2[attrname];
            }
        }
    }

}

/*   usage		******
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

/******/

/*

Writing classes which use noveltheory.EventSource

  NOTE: noveltheory.EventSource was written using Typescript,
   and intended to be used in classes written in TypeScript.
   Using it in pure JavaScript code is possible, but quite 
   tricky, due to the arcane was object inheritance it handled.

Inherit from the noveltheory.EventSource base class

	class MyClass extends noveltheory.EventSource
	{

In the ctor, call the superclass.

	constructor()
	{
		super();

Also in the ctor, you may optionally define what event are handled.
If any are specified, on() will throw an exception if any other \
 string is given for type.
If none are specified, on() will accept any string.

		this.allowedEvents.push("changed");
	}

In a method of class, call trigger:

	edit()
	{
		if (hasChanged)
			this.trigger("changed"); 
	}


*/