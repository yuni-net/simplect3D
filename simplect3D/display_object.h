#pragma once

namespace si3
{
	class display_object
	{
	public:
		virtual void draw_no_alpha() const=0;
		virtual void draw_alpha() const=0;
	};
}