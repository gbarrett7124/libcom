Most of the time when dealing with COM objects, you deal with interface pointers: at the lowest level, this is an `IUnknown *` (or a pointer to some other interface which is descended from `IUnknown`). At some stage, you will need to actually create an instance of some class in order to get at that interface pointer.

Instances of new COM objects are only ever directly created by something with knowledge of the internals of the class: for classes managed internally by the COM runtime, you make API calls and get an interface pointer back. A good example of this is `CoTaskMemAlloc`, which returns an instance of the runtime-provided memory allocator—all you know about it is that it implements `IMalloc`, but behind the scenes an instance of the actual class is created. COM is structured in such a way that you don't need to know about the nitty gritty of how that memory allocator instance is created—provided it does get created (or is statically defined), and supports `IMalloc`, and an `IMalloc` interface pointer for an instance can be returned to you when you request it, your code is happy.

Calling APIs and getting interface pointers back is straightforward, but only tells part of the story: if a particular class GUID has been properly registered, you can cause an instance of that class to be created and returned to you. There are two upshots to this: the first is that you don't need predefined APIs for every class you want to create (this would rather defeat the purpose of interfaces), and the second is that the GUID doesn't have to be known until runtime (provided the class implements the interface you want, you shouldn't need to care when you’re compiling your application what specific implementation is being used).

The generic instance-creation process is a multi-stage one.

The server needs to be registered—this is what the Windows Registry is for. The registry entries for the particular GUID (under HKEY\_CLASSES\_ROOT\CLSID\

&lt;xxx&gt;

) contain all the information required for the COM runtime to ask the server for what it needs. There are various APIs and pieces of documentation on how to register a server—it varies depending on what kind of server it is (COM classes in running applications are dealt with differently to DLLs which are loaded into the client process, for example).

Once registered, you can ask COM to give you an interface pointer on the server’s class factory. The class factory is a special class which is the broad equivalent of a constructor: when you register a class, it's actually the _class factory_ that gets registered. A class factory is usually very simple, so that when the COM runtime requests an interface on it, there isn't much work to do.

A lot of people get confused about this, though—if COM is able to get a class factory, why can't it just get the class you want in the first place?

The answer lies in flexibility.