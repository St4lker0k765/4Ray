#pragma once

namespace vfs
{
	class tracer
	{
	protected:
		str_shared _types[26];
		str_shared _output_dir;
		str_shared _map_name;

		bool _enabled;
	public:
		tracer();
		tracer(tracer* other);
		virtual ~tracer();

		void begin();
		void end();


	};
}