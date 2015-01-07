/*
 * Hyde, lightweight HID controllers.
 * Copyright (c) 2010,2011,2012,2013 Mario 'rlyeh' Rodriguez

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

 * @todo:
 * - N should be seconds time rather than polling samples
 * - Send 'off' to buttons,lists,etc when windows focus is lost
 * - Proposal:
 *   at(   0) -> current value
 *   at(-0.5) -> lerp value 0.5 seconds ago (stats)
 *   at(+0.5) -> prediction value in 0.5 seconds

 * - rlyeh ~~ listening to My Brother The Wind / Death and Beyond
 */

#pragma once

#include <cassert>

#include <algorithm>
#include <chrono>
#include <deque>
#include <vector>

// Platform independent key codes

#ifdef _WIN32
#   include <winsock2.h>
#   define hyde$keycode( windows, linux, apple ) windows
#endif

#if defined(__linux__) || defined(__unix__)
#   define hyde$keycode( windows, linux, apple ) linux
#endif

#ifdef __APPLE__
#   define hyde$keycode( windows, linux, apple ) apple
#endif

#ifndef hyde$keycode
#   error  define a platform
#endif

namespace hyde
{
    // @todo: add credit: original keycode list by ...?
    // found this: http://www.meandmark.com/keycodes.html

    struct keycode //portable_keycodes
    {
        enum enumeration
        {
            A          = hyde$keycode( int('A'),      int('a'),        0x00 ),
            B          = hyde$keycode( int('B'),      int('b'),        0x0B ),
            C          = hyde$keycode( int('C'),      int('c'),        0x08 ),
            D          = hyde$keycode( int('D'),      int('d'),        0x02 ),
            E          = hyde$keycode( int('E'),      int('e'),        0x0E ),
            F          = hyde$keycode( int('F'),      int('f'),        0x03 ),
            G          = hyde$keycode( int('G'),      int('g'),        0x05 ),
            H          = hyde$keycode( int('H'),      int('h'),        0x04 ),
            I          = hyde$keycode( int('I'),      int('i'),        0x22 ),
            J          = hyde$keycode( int('J'),      int('j'),        0x26 ),
            K          = hyde$keycode( int('K'),      int('k'),        0x28 ),
            L          = hyde$keycode( int('L'),      int('l'),        0x25 ),
            M          = hyde$keycode( int('M'),      int('m'),        0x2E ),
            N          = hyde$keycode( int('N'),      int('n'),        0x2D ),
            O          = hyde$keycode( int('O'),      int('o'),        0x1F ),
            P          = hyde$keycode( int('P'),      int('p'),        0x23 ),
            Q          = hyde$keycode( int('Q'),      int('q'),        0x0C ),
            R          = hyde$keycode( int('R'),      int('r'),        0x0F ),
            S          = hyde$keycode( int('S'),      int('s'),        0x01 ),
            T          = hyde$keycode( int('T'),      int('t'),        0x11 ),
            U          = hyde$keycode( int('U'),      int('u'),        0x20 ),
            V          = hyde$keycode( int('V'),      int('v'),        0x09 ),
            W          = hyde$keycode( int('W'),      int('w'),        0x0D ),
            X          = hyde$keycode( int('X'),      int('x'),        0x07 ),
            Y          = hyde$keycode( int('Y'),      int('y'),        0x10 ),
            Z          = hyde$keycode( int('Z'),      int('z'),        0x06 ),

            ONE        = hyde$keycode( int('1'),      int('1'),        int('1') ),
            TWO        = hyde$keycode( int('2'),      int('2'),        int('2') ),
            THREE      = hyde$keycode( int('3'),      int('3'),        int('3') ),
            FOUR       = hyde$keycode( int('4'),      int('4'),        int('4') ),
            FIVE       = hyde$keycode( int('5'),      int('5'),        int('5') ),
            SIX        = hyde$keycode( int('6'),      int('6'),        int('6') ),
            SEVEN      = hyde$keycode( int('7'),      int('7'),        int('7') ),
            EIGHT      = hyde$keycode( int('8'),      int('8'),        int('8') ),
            NINE       = hyde$keycode( int('9'),      int('9'),        int('9') ),
            ZERO       = hyde$keycode( int('0'),      int('0'),        int('0') ),

            ESCAPE     = hyde$keycode( VK_ESCAPE,     XK_Escape,       0x35 ),
            BACKSPACE  = hyde$keycode( VK_BACK,       XK_BackSpace,    0x33 ),
            TAB        = hyde$keycode( VK_TAB,        XK_Tab,          0x30 ),
            ENTER      = hyde$keycode( VK_RETURN,     XK_Return,       0x24 ),
            SHIFT      = hyde$keycode( VK_SHIFT,      XK_Shift_L,      0x38 ), //R?
            CTRL       = hyde$keycode( VK_CONTROL,    XK_Control_L,    0x3B ), //R?
            SPACE      = hyde$keycode( VK_SPACE,      XK_space,        0x31 ),
            ALT        = hyde$keycode( VK_LMENU,      XK_Alt_L,        0x3A ), //R?

            UP         = hyde$keycode( VK_UP,         XK_Up,           0x7E ),
            DOWN       = hyde$keycode( VK_DOWN,       XK_Down,         0x7D ),
            LEFT       = hyde$keycode( VK_LEFT,       XK_Left,         0x7B ),
            RIGHT      = hyde$keycode( VK_RIGHT,      XK_Right,        0x7C ),
            HOME       = hyde$keycode( VK_HOME,       XK_Home,         0x73 ),
            END        = hyde$keycode( VK_END,        XK_End,          0x77 ),
            INSERT     = hyde$keycode( VK_INSERT,     XK_Insert,       0x72 ),
            DEL        = hyde$keycode( VK_DELETE,     XK_Delete,       0x33 ),

            F1         = hyde$keycode( VK_F1,         XK_F1,           0x7A ),
            F2         = hyde$keycode( VK_F2,         XK_F2,           0x78 ),
            F3         = hyde$keycode( VK_F3,         XK_F3,           0x63 ),
            F4         = hyde$keycode( VK_F4,         XK_F4,           0x76 ),
            F5         = hyde$keycode( VK_F5,         XK_F5,           0x60 ),
            F6         = hyde$keycode( VK_F6,         XK_F6,           0x61 ),
            F7         = hyde$keycode( VK_F7,         XK_F7,           0x62 ),
            F8         = hyde$keycode( VK_F8,         XK_F8,           0x64 ),
            F9         = hyde$keycode( VK_F9,         XK_F9,           0x65 ),
            F10        = hyde$keycode( VK_F10,        XK_F10,          0x6D ),
            F11        = hyde$keycode( VK_F11,        XK_F11,          0x67 ),
            F12        = hyde$keycode( VK_F12,        XK_F12,          0x6F ),

            NUMPAD1    = hyde$keycode( VK_NUMPAD1,    XK_KP_1,         0x53 ),
            NUMPAD2    = hyde$keycode( VK_NUMPAD2,    XK_KP_2,         0x54 ),
            NUMPAD3    = hyde$keycode( VK_NUMPAD3,    XK_KP_3,         0x55 ),
            NUMPAD4    = hyde$keycode( VK_NUMPAD4,    XK_KP_4,         0x56 ),
            NUMPAD5    = hyde$keycode( VK_NUMPAD5,    XK_KP_5,         0x57 ),
            NUMPAD6    = hyde$keycode( VK_NUMPAD6,    XK_KP_6,         0x58 ),
            NUMPAD7    = hyde$keycode( VK_NUMPAD7,    XK_KP_7,         0x59 ),
            NUMPAD8    = hyde$keycode( VK_NUMPAD8,    XK_KP_8,         0x5B ),
            NUMPAD9    = hyde$keycode( VK_NUMPAD9,    XK_KP_9,         0x5C ),
            NUMPAD0    = hyde$keycode( VK_NUMPAD0,    XK_KP_0,         0x52 ),

            ADD        = hyde$keycode( VK_ADD,        XK_KP_Add,       0x45 ),
            SUBTRACT   = hyde$keycode( VK_SUBTRACT,   XK_KP_Subtract,  0x4E ),
            MULTIPLY   = hyde$keycode( VK_MULTIPLY,   XK_KP_Multiply,  0x43 ),
            DIVIDE     = hyde$keycode( VK_DIVIDE,     XK_KP_Divide,    0x4B ),
            SEPARATOR  = hyde$keycode( VK_SEPARATOR,  XK_KP_Separator, 0x2B ),
            DECIMAL    = hyde$keycode( VK_DECIMAL,    XK_KP_Decimal,   0x41 )
            //PAUSE    = hyde$keycode( VK_PAUSE,      XK_Pause,        ERROR )
        };
    };

    struct mousebuttons
    {
        enum enumeration
        {
            LEFT,
            MIDDLE,
            RIGHT
        };
    };

    struct mouseflag
    {
        enum enumeration
        {
            HOVER,
            IS_CONNECTED
        };
    };

}

#undef hyde$keycode


namespace hyde
{
    namespace hid
    {
        class dt
        {
            typedef std::chrono::high_resolution_clock clock;
            clock::time_point start;

            public:

            dt()
            {
                start = clock::now();
            }

            void reset()
            {
                *this = dt();
            }

            double s()
            {
                return std::chrono::nanoseconds( clock::now() - start ).count() / 1000000000.0; //::seconds
            }

            double ms()
            {
                return std::chrono::nanoseconds( clock::now() - start ).count() / 1000000.0; //::milliseconds
            }

            double us()
            {
                return std::chrono::nanoseconds( clock::now() - start ).count() / 1000.0; //::microseconds
            }

            double ns()
            {
                return std::chrono::nanoseconds( clock::now() - start ).count() / 1.0;
            }
        };
    }

    extern hyde::hid::dt global_timer;

    template< typename SAMPLE_TYPE, const int N = 120 >
    class history : public SAMPLE_TYPE
    {
        // N samples = fixed deque of N samples
        //
        // (t0,sample0) (t1,sample1) ... (tN-1,sampleN-1)
        // newest...oldest
        //
        // now = find_dt( dt = 0 ) -> iter #0
        // Nth = find_dt( dt = 1 ) -> iter #N-1
        //
        // now = find_t( t = 0 secs ago ) -> iter #0
        // then = find_t( t = T secs ago ) -> iter #N-1
        //
        // duration = t0 - tN
        //

        // todo: catmull-rom/math9::stats lerp for previous positions (useful?)
        // todo: burg to predict next positions (useful?)

        std::deque< SAMPLE_TYPE > container;

        public:

        history() : container( N )
        {
            clear();
            std::reverse( container.begin(), container.end() );
        }

        size_t size() const
        {
            return container.size();
        }

        void clear()
        {
            double s = global_timer.s();

            for( size_t i = 0; i < N; ++i )
                container.at(i).t = s;
        }

        typename std::deque< SAMPLE_TYPE >::const_iterator newest_it() const   // ~recent(), ~current()
        {
            assert( container.size() );
            return container.begin();
        }
/* TO_DEPRECATE
        typename std::deque< SAMPLE_TYPE >::const_iterator oldest_it() const
        {
            assert( container.size() );
            return container.end() - 1;
        }
*/
        const SAMPLE_TYPE &newest() const
        {
            return *newest_it();
        }
/* TO_DEPRECATE
        const SAMPLE_TYPE &oldest() const
        {
            return *oldest_it();
        }
*/
        const double duration() const
        {
            return container.at(1).t - container.back().t;
        }

        typename std::deque< SAMPLE_TYPE >::const_iterator find_dt( const double &dt01 ) const
        {
            // 0 = newest ... 1 = oldest

            assert( container.size() );

            size_t dtpos = size_t( dt01 * container.size() );

            if( dtpos >= container.size() )
                dtpos = container.size() - 1;

            return container.begin() + dtpos;
        }
        typename std::deque< SAMPLE_TYPE >::const_iterator find_t( const double &seconds_ago ) const
        {
            // if seconds_ago_lapse <= 0 then return newest() iterator
            // if seconds_ago_lapse = ~0 then return iterator(s) close to newest()
            // ...
            // if seconds_ago_lapse = ~N then return iterator(s) close to oldest()
            // if seconds_ago_lapse >= N then return oldest() iterator

            assert( container.size() );

            if( seconds_ago <= 0 )
                return begin();

            if( seconds_ago >= duration() )
                return end() - 1;

            double current_time = begin()->t;
            double time_target = current_time - seconds_ago;

            const_iterator it;

            for( it = container.begin(); it != container.end(); ++it )
                if( time_target >= it->t )
                    return it;

            return end() - 1;
        }

        const SAMPLE_TYPE &then_dt( const double &dt ) const
        {
            return *find_dt( dt );
        }
        const SAMPLE_TYPE &then_t( const double &seconds_ago_lapse ) const
        {
            return *find_t( seconds_ago_lapse );
        }

        history interval_dt( const double &from_01, const double &to_01 ) const
        {
            const_iterator from = find_dt( from_01 );
            const_iterator to = find_dt( to_01 );

            assert( (to - from) && "invalid interval" );

            size_t N = to - from;

            history h( N );

            std::copy( from, to, h.begin() );

            return h;
        }
        history interval_t( const double &from_t, const double &to_t ) const
        {
            const_iterator from = find_t( from_t );
            const_iterator to = find_t( to_t );

            assert( (to - from) && "invalid interval" );

            size_t N = to - from;

            history h( N );

            std::copy( from, to, h.begin() );

            return h;
        }

        private:

        void update_timestamp( int pos )
        {
            container.at( pos ).t = global_timer.s();
        }

        //public:

        // set: function that push_front's element when relevant

        void set( const SAMPLE_TYPE &new_sample )
        {
            if( new_sample == container.front() )   // update timestamp if value same than previous (~rle), insert if new value is relevant ~treshold
            {
                update_timestamp(0); //y del resto... //useful?
            }
            else
            {
                // @todo: hago el resto... useful?
                //update_timestamp(N-1);
                //...
                //update_timestamp(1);
                update_timestamp(0);

                #if 1
                // @todo: keep treshold and other members from current deque
                container.push_front( container.front() );
                container.front().set( new_sample );
                #else
                container.push_front( new_sample );
                #endif
                container.front().t = global_timer.s();

                if( container.size() > N )
                    container.pop_back();
            }

            import( newest() );
        }

        public:

        // sugars{

        template <typename T>
        void set( const T &t0 )
        {
            set( SAMPLE_TYPE(t0) );
        }

        template <typename T>
        void set( const T &t0, const T &t1 )
        {
            set( SAMPLE_TYPE(t0,t1) );
        }

        template <typename T>
        void set( const T &t0, const T &t1, const T &t2 )
        {
            set( SAMPLE_TYPE(t0,t1,t2) );
        }



        //}

        // pattern matching {

            // @todo
            // los patterns estaria mejor que dieran un gestalt o un valor aproximado segun encuentren
            // un patron o no. tiene mas sentido; mejor que devolver un booleano. ademas enlazo ya
            // con los gestures : )

#if 1
            // idle: low [then] -> low [now]
            bool idle( float interval_t = 0.0125f ) //interval useful here?
            {
                double then_t = container.at(1).t;
                double  now_t = container.at(0).t;

                if( now_t - then_t > interval_t )
                    return false; //time exceeded

                double then = container.at(1).x;
                double  now = container.at(0).x;

                return then < 0.5f && now < 0.5f;
            }
#else
            // original code. conflict with release()
            // idle: low [now]
            bool idle()
            {
                double now = container.at(0).x;

                return now < 0.5f;
            }
#endif

            // trigger: [then] low -> high [now]
            bool trigger( float interval_t = 0.0125f ) //interval useful here?
            {
                double then_t = container.at(1).t;
                double  now_t = container.at(0).t;

                if( now_t - then_t > interval_t )
                    return false; //time exceeded

                double then = container.at(1).x;
                double  now = container.at(0).x;

                return then < 0.5f && now >= 0.5f;
            }

            // hold: high [now]
            bool hold()
            {
                double now = container.at(0).x;

                return now >= 0.5f;
            }

            // release: [then] high -> low [now]
            bool release( float interval_t = 0.0125f ) //interval useful here?
            {
                double then_t = container.at(1).t;
                double  now_t = container.at(0).t;

                if( now_t - then_t > interval_t )
                    return false; //time exceeded

                double then = container.at(1).x;
                double  now = container.at(0).x;

                return then >= 0.5f && now < 0.5f;
            }

            // click: [then] low -> high -> low [now]       // also: peak, tap
            bool click( float interval_t = 0.500f )
            {
                double then_t = container.at(2).t;
                double  now_t = container.at(0).t;

                if( now_t - then_t > interval_t )
                    return false; //time exceeded

                double then = container.at(2).x;
                double  mid = container.at(1).x;
                double  now = container.at(0).x;

                return then < 0.5f && mid >= 0.5f && now < 0.5f;
            }

            // click: [then] low -> high -> low -> high -> low [now]
            bool dclick( float interval_t = 0.500f )
            {
                double then_t = container.at(4).t;
                double  now_t = container.at(0).t;

                if( now_t - then_t > interval_t )
                    return false; //time exceeded

                double then = container.at(4).x;
                double mid1 = container.at(3).x;
                double  mid = container.at(2).x;
                double mid2 = container.at(1).x;
                double  now = container.at(0).x;

                bool b0 = then >= 0.5f;
                bool b1 = mid1 >= 0.5f;
                bool b2 =  mid >= 0.5f;
                bool b3 = mid2 >= 0.5f;
                bool b4 =  now >= 0.5f;

                return then < 0.5f && mid1 >= 0.5f && mid < 0.5f && mid2 >= 0.5f && now < 0.5f;
            }

        // }
    };
}

namespace hyde
{
    namespace types
    {
        namespace hid
        {
            struct string
            {
                double t;
                std::string x;
            };

            template <typename T>
            struct vec1
            {
                double t;
                float treshold;
                T x, xdt;

                vec1( const T &t0 = T() ) : t(0),treshold(0.0f),x(t0),xdt(0) {}
                vec1( const vec1 &v ) { operator=(v); }
                vec1 &operator =( const vec1 &v ) { if( this != &v ) t = v.t, treshold = v.treshold, x = v.x, xdt = v.xdt; return *this; }
                void set( const vec1 &v ) { xdt = v.x - x; x = v.x; }
                const bool operator ==( const vec1 &v ) const { return std::abs( x - v.x ) < treshold; }
                void import( const vec1 &v )
                    { operator=( v ); };
            };

            template <typename T>
            struct vec2
            {
                double t;
                float treshold;
                T x, y, xdt, ydt;

                vec2( const T &t0 = T(), const T &t1 = T() ) : t(0),treshold(0.0f),x(t0),y(t1),xdt(0),ydt(0) {}
                vec2( const vec2 &v ) { operator=(v); }
                vec2 &operator =( const vec2 &v ) { if( this != &v ) t = v.t, treshold = v.treshold, x = v.x, y = v.y, xdt = v.xdt, ydt = v.ydt; return *this; }
                void set( const vec2 &v ) { xdt = v.x - x, ydt = v.y - y; x = v.x, y = v.y; }
                const bool operator ==( const vec2 &v ) const { return std::abs( x - v.x ) < treshold && std::abs( y - v.y ) < treshold; }
                void import( const vec2 &v )
                    { operator=( v ); };
            };

            template <typename T>
            struct vec3
            {
                double t;
                float treshold;
                T x, y, z, xdt, ydt, zdt;

                vec3( const T &t0 = T(), const T &t1 = T(), const T &t2 = T() ) : t(0),treshold(0.0f),x(t0),y(t1),z(t2),xdt(0),ydt(0),zdt(0) {}
                vec3( const vec3 &v ) { operator=(v); }
                vec3 &operator =( const vec3 &v ) { if( this != &v ) t = v.t, treshold = v.treshold, x = v.x, y = v.y, z = v.z, xdt = v.xdt, ydt = v.ydt, zdt = v.zdt; return *this; }
                void set( const vec3 &v ) { xdt = v.x - x, ydt = v.y - y, zdt = v.z - z; x = v.x, y = v.y, z = v.z; }
                const bool operator ==( const vec3 &v ) const { return std::abs( x - v.x ) < treshold && std::abs( y - v.y ) < treshold && std::abs( z - v.z ) < treshold; }
                void import( const vec3 &v )
                    { operator=( v ); };
            };
        }

        namespace wip_hid
        {
            // normalized data [ -1, +1 ] for HIDs (human interface devices)

            template <const int N, typename T = float>
            struct vec
            {
                T elem[ N ];

                vec()
                    { for( size_t i = 0; i < N; ++i ) elem[ i ] = 0; }
                vec( const vec<N,T> &v )
                    { operator=( v.elem ); }
                vec( const T *v )
                    { operator=( v ); }

                void operator =( const T *v )
                    { for( size_t i = 0; i < N; ++i ) elem[ i ] = v[ i ]; }

                operator T*()
                    { return elem; }
                operator const T*() const
                    { return elem; }

                T magnitude() const
                    { T dot = 0; for( size_t i = 0; i < N; ++i ) dot += elem[ i ] * elem[ i ]; return (T)sqrtf( (float)dot ); }
            };

            template <const int N>
            struct vec<N, double>
            {
                double magnitude() const
                    { double dot = 0; for( size_t i = 0; i < N; ++i ) dot += elem[ i ] * elem[ i ]; return sqrt( dot ); }
            };

            struct vec1 : public vec<1,float>
            {
                float &x;

                vec1( float X = 0 ) : x(elem[0])
                    { elem[0] = X; }
                vec1( const vec<1,float> &rhs ) : vec<1,float>(rhs), x(elem[0])
                    {}
                vec1( const vec1 &rhs ) : vec<1,float>(rhs), x(elem[0])
                    {}
                explicit vec1( const float *elements ) : vec<1,float>(elements), x(elem[0])
                    {}

                void operator=( const vec1 &v )
                    { elem[0] = v.elem[0]; }
                void import( const vec1 &v )
                    { operator=( v ); };
            };

            struct vec2 : public vec<2,float>
            {
                float &x, &y;

                vec2( float X = 0, float Y = 0 ) : x(elem[0]), y(elem[1])
                    { elem[0] = X, elem[1] = Y; }
                vec2( const vec<2,float> &rhs ) : vec<2,float>(rhs), x(elem[0]), y(elem[1])
                    {}
                vec2( const vec2 &rhs ) : vec<2,float>(rhs), x(elem[0]), y(elem[1])
                    {}
                explicit vec2( const float *elements ) : vec<2,float>(elements), x(elem[0]), y(elem[1])
                    {}

                void operator=( const vec2 &v )
                    { elem[0] = v.elem[0], elem[1] = v.elem[1]; }
                void import( const vec2 &v )
                    { operator=( v ); };
            };

            struct vec3 : public vec<3,float>
            {
                float &x, &y, &z;

                vec3( float X = 0, float Y = 0, float Z = 0 ) : x(elem[0]), y(elem[1]), z(elem[2])
                    { elem[0] = X, elem[1] = Y, elem[2] = Z; }
                vec3( const vec2 &xy, float z = 0 ) : x(elem[0]), y(elem[1]), z(elem[2])
                    { elem[0] = xy[0], elem[1] = xy[1], elem[2] = z; }
                vec3( const vec<3,float> &rhs ) : vec<3,float>(rhs), x(elem[0]), y(elem[1]), z(elem[2])
                    {}
                vec3( const vec3 &rhs ) : vec<3,float>(rhs), x(elem[0]), y(elem[1]), z(elem[2])
                    {}
                explicit vec3( const float *elements ) : vec<3,float>(elements), x(elem[0]), y(elem[1]), z(elem[2])
                    {}

                void operator=( const vec3 &v )
                    { elem[0] = v.elem[0], elem[1] = v.elem[1], elem[2] = v.elem[2]; }
                void import( const vec3 &v )
                    { operator=( v ); };

                // conversions
                const vec2 &xy() const
                    { return *reinterpret_cast<const vec2 *>(elem); }
                vec2 &xy()
                    { return *reinterpret_cast<vec2 *>(elem); }
            };

            struct vec4 : public vec<4,float>
            {
                float &x, &y, &z, &w;

                vec4( float X = 0, float Y = 0, float Z = 0, float W = 0 ) : x(elem[0]), y(elem[1]), z(elem[2]), w(elem[3])
                    { elem[0] = X, elem[1] = Y, elem[2] = Z, elem[3] = W; }
                vec4( const vec2 &xy, float Z = 0, float W = 0) : x(elem[0]), y(elem[1]), z(elem[2]), w(elem[3])
                    { elem[0] = xy[0], elem[1] = xy[1], elem[2] = Z, elem[3] = W; }
                vec4( const vec3 &xyz, float W = 0) : x(elem[0]), y(elem[1]), z(elem[2]), w(elem[3])
                    { elem[0] = xyz[0], elem[1] = xyz[1], elem[2] = xyz[2], elem[3] = W; }
                vec4( const vec<4,float> &rhs ) : vec<4,float>(rhs), x(elem[0]), y(elem[1]), z(elem[2]), w(elem[3])
                    {}
                vec4( const vec4 &rhs ) : vec<4,float>(rhs), x(elem[0]), y(elem[1]), z(elem[2]), w(elem[3])
                    {}

                explicit vec4( const float *elements ) : vec<4,float>(elements), x(elem[0]), y(elem[1]), z(elem[2]), w(elem[3])
                    {}

                void operator=( const vec4 &v )
                    { elem[0] = v.elem[0], elem[1] = v.elem[1], elem[2] = v.elem[2], elem[3] = v.elem[3]; }
                void import( const vec4 &v )
                    { operator=( v ); };

                // conversions
                const vec2 &xy() const
                    { return *reinterpret_cast<const vec2 *>(elem); }
                vec2 &xy()
                    { return *reinterpret_cast<vec2 *>(elem); }

                const vec3 &xyz() const
                    { return *reinterpret_cast<const vec3 *>(elem); }
                vec3 &xyz()
                    { return *reinterpret_cast<vec3 *>(elem); }
            };
        }
    }

    typedef hyde::history< types::hid::vec1<float> > flag;
    typedef hyde::history< types::hid::vec1<float> > key;
    typedef hyde::history< types::hid::vec1<float> > button;
    typedef hyde::history< types::hid::vec2<float> > coordinate;        //axis2d? xy?
    typedef hyde::history< types::hid::vec3<float> > axis;              //axis3d? xyz?
    typedef hyde::history< types::hid::     string > serializer;

    typedef std::vector<          flag > flags;
    typedef std::vector<           key > keys;
    typedef std::vector<        button > buttons;
    typedef std::vector<    coordinate > coordinates;
    typedef std::vector<          axis > axes;
    typedef std::vector<    serializer > serializers;
}



#ifdef _WIN32

#include <limits>
#include <set>

#include <cassert>
#include <iostream>

#include <map>

#include <cassert>
#include <winsock2.h>
#include <Windows.h>
#include <mmsystem.h>                   // joystick
#include <XInput.h>                     // gamepad360

#pragma comment(lib, "winmm.lib")       // joystick
#pragma comment(lib, "XInput.lib")      // gamepad360
#pragma comment(lib, "user32.lib")      // GetAsyncKeyState [...]

#include "deps/manymouse/manymouse.h"

namespace hyde
{
    class sharing_policy
    {
        public:

        template<typename T>
        static T *get_master( T &controller, size_t id , bool is_creating )
        {
            // quick auto-creation map
            struct tid
            {
                T *master;  //shared_ptr
                size_t id, num_instances;
                tid( size_t _id = 0 ) : master(0), id(_id), num_instances(0) {}
                const bool operator <( const tid &t ) const { return id < t.id; }
            };

            static std::map<tid,tid> map;

            tid &ref_id = ( map[tid(id)] = map[tid(id)] );

            if( is_creating )
            {
                if( ref_id.num_instances + 1 <= 1 )
                {
                    ref_id.master = &controller;
                    ++ref_id.num_instances;
                }
                //else //throw hyde::string( "error: device id #\1 already in use! (seen at \2 controller)", id, controller.typeof );
            }
            else
            {
                --ref_id.num_instances;
            }

            return ref_id.master;
        }
    };

    namespace windows_wip
    {

        class joystick
        {
            UINT joy_id;

            public:

            hyde::flag is_ready;
            hyde::coordinate xy;
            hyde::buttons buttons;

            // http://www.gnu-darwin.org/www001/src/ports/graphics/gephex/work/gephex-0.4.3/util/src/libjoystick/win32joystickdriver.cpp
            // http://www.gnu-darwin.org/www001/src/ports/graphics/gephex/work/gephex-0.4.3/util/src/libjoystick/

            joystick( size_t id )
            {
                assert( id <  2 && "invalid joystick id" ); /* // nt [0.. 1]
                assert( id < 16 && "invalid joystick id" ); */ // xp [0..15]

                joy_id = JOYSTICKID1 + id;

                // calibrate
                // WinExec("control joy.cpl", SW_NORMAL);

                /*
                MMRESULT mr;
                JOYCAPS jc;

                mr = joyGetDevCaps(handle, &jc, sizeof(jc));

                if (mr != JOYERR_NOERROR)
                {
                    throw std::runtime_error("Could not get caps for joystick");
                }

                m_num_axes      = jc.wNumAxes;
                m_num_buttons   = jc.wNumButtons;
                m_state.axes    = std::vector<int>(m_num_axes, 0);
                m_state.buttons = std::vector<bool>(m_num_buttons, false);
                */
            }

            void clear()
            {
                is_ready.clear();
                xy.clear();

                for( auto &it : buttons )
                    it.clear();
            }

            void update()
            {
                if( !GetFocus() )
                {
                    if( GetForegroundWindow() != GetConsoleWindow() )
                    {
                        clear();
                        return;
                    }
                }

                //typedef struct joyinfoex_tag {
                //    DWORD dwSize;                /* size of structure */
                //    DWORD dwFlags;               /* flags to indicate what to return */
                //    DWORD dwXpos;                /* x position */
                //    DWORD dwYpos;                /* y position */
                //    DWORD dwZpos;                /* z position */ <-- throttle
                //    DWORD dwRpos;                /* rudder/4th axis position */ <-- pedals
                //    DWORD dwUpos;                /* 5th axis position */
                //    DWORD dwVpos;                /* 6th axis position */
                //    DWORD dwButtons;             /* button states */
                //    DWORD dwButtonNumber;        /* current button number pressed */
                //    DWORD dwPOV;                 /* point of view state */
                //    DWORD dwReserved1;           /* reserved for communication between winmm & driver */
                //    DWORD dwReserved2;           /* reserved for future expansion */
                //} JOYINFOEX, *PJOYINFOEX, NEAR *NPJOYINFOEX, FAR *LPJOYINFOEX;

                JOYCAPS joyCaps;
                joyGetDevCaps(joy_id, &joyCaps, sizeof(joyCaps));
                BOOL has_throttle = (joyCaps.wCaps & JOYCAPS_HASZ);

                JOYINFOEX joyInfoEx;
                ZeroMemory(&joyInfoEx, sizeof(joyInfoEx));

                joyInfoEx.dwSize = sizeof(joyInfoEx);
                joyInfoEx.dwFlags = JOY_RETURNALL | JOY_RETURNCENTERED | JOY_USEDEADZONE | (has_throttle ? JOY_RETURNZ : 0);

                BOOL is_present = (joyGetPosEx(joy_id, &joyInfoEx) == JOYERR_NOERROR);
                is_ready.set( is_present ? 1.f : 0.f );

                if( is_present )
                {
                    // vec6 !
                    /*
                    axes[0] = joyInfoEx.dwXpos;
                    axes[1] = joyInfoEx.dwYpos;
                    if (m_num_axes > 2)
                        m_state.axes[2] = joyInfoEx.dwZpos; // throttle? sure?
                    if (m_num_axes > 3)
                        m_state.axes[3] = joyInfoEx.dwRpos;
                    if (m_num_axes > 4)
                        m_state.axes[4] = joyInfoEx.dwUpos;
                    if (m_num_axes > 5)
                        m_state.axes[5] = joyInfoEx.dwVpos;
                    */

                    size_t num_buttons = buttons.size();
                    for( size_t i = 0; i < num_buttons; ++i )
                        buttons[i].set( (joyInfoEx.dwButtons & (JOY_BUTTON1+i) ) ? 1 : 0 );
                }
            }
        };
    }

    namespace windows
    {
        class gamepad
        {
            private:

                XINPUT_STATE state;
                size_t id;

            public:

                // pad pieces

                // 10x buttons,   1d data input (action)
                //  2x triggers,  1d data input (action)
                hyde::button
                    a, b, x, y,
                    back, start,
                    lb, rb,
                    lthumb, rthumb,
                    ltrigger, rtrigger;

                //  2x axis,      2d data input (spatial)
                //  1x gamepad,   2d data input (spatial)
                hyde::coordinate
                    pad,
                    lpad, rpad;

                //  1x mic,       1d data output (mono audio)
                hyde::button
                    mic;

                //  1x earphones, 2d data output (stereo audio)
                hyde::coordinates
                    earphones;

                // 47x keys,      1d data input (text)
                hyde::keys
                    keymap;

                // 2x rumble,     1d data output (rumble haptic)
                hyde::buttons rumble;

                hyde::flag
                    is_ready;   // ~? is_present, is_plugged, is_connected

                gamepad *master;

            gamepad( const size_t &_id ) :
                id(_id),
                keymap(47),
                rumble(2),
                typeof( "hyde::windows::gamepad" )
            {

                if( id >= max_devices )
                {
                    std::cerr << "error: device id #" << id << " invalid! (seen at " << typeof << " controller)" << std::endl;
                    assert( false );
                    return;
                }

                master = sharing_policy::get_master( *this, id, true );

                if( master == this )
                    set_rumble( 0.f, 0.f );

//                ltrigger.newest().deadzone = 0.1f;
//                rtrigger.newest().deadzone = 0.1f;
            }

            ~gamepad()
            {
                if( master == this )
                    set_rumble( 0.f, 0.f );

                // check & decrement instance counter
                master = sharing_policy::get_master( *this, id, false );
            }

            const char *const typeof;
            static const size_t max_devices = 4;

            protected:

            XINPUT_GAMEPAD get_state()
            {
                ZeroMemory(&state, sizeof(XINPUT_STATE));

                XInputGetState(id, &state);

                return state.Gamepad;
            }

            void set_rumble( const float &left01, const float &right01 )
            {
                assert( left01 >= 0.f && left01 <= 1.f );
                assert( right01 >= 0.f && right01 <= 1.f );

                //if(!is_ready()) return;

                XINPUT_VIBRATION vibration_state;
                ZeroMemory( &vibration_state, sizeof( XINPUT_VIBRATION ) );

                vibration_state.wLeftMotorSpeed  = DWORD(  left01 * 65535.f );
                vibration_state.wRightMotorSpeed = DWORD( right01 * 65535.f );

                XInputSetState( id, &vibration_state );
            }

            public:

            void clear()
            {
                a.clear();
                b.clear();
                x.clear();
                y.clear();
                back.clear();
                start.clear();
                lb.clear();
                rb.clear();
                lthumb.clear();
                rthumb.clear();
                ltrigger.clear();
                rtrigger.clear();
                pad.clear();
                lpad.clear();
                rpad.clear();
                mic.clear();
                is_ready.clear();

                for( auto &it : earphones )
                    it.clear();

                for( auto &it : keymap )
                    it.clear();

                for( auto &it : rumble )
                    it.clear();
            }

            void update()
            {
                if( master != this )
                {
                    a = master->a;
                    b = master->b;
                    x = master->x;
                    y = master->y;
                    back = master->back;
                    start = master->start;
                    lb = master->lb;
                    rb = master->rb;
                    lthumb = master->lthumb;
                    rthumb = master->rthumb;
                    ltrigger = master->ltrigger;
                    rtrigger = master->rtrigger;
                    pad = master->pad;
                    lpad = master->lpad;
                    rpad = master->rpad;
                    mic = master->mic;
                    earphones = master->earphones;
                    keymap = master->keymap;
                    rumble = master->rumble;
                    is_ready = master->is_ready;

                    return;
                }

                if( !GetFocus() )
                {
                    if( GetForegroundWindow() != GetConsoleWindow() )
                    {
                        clear();
                        return;
                    }
                }

                ZeroMemory(&state, sizeof(XINPUT_STATE));
                bool is_present = ( XInputGetState( id, &state ) == ERROR_SUCCESS );

                is_ready.set( is_present );

                if( !is_present )
                    return;

                // read device
                XINPUT_GAMEPAD state = get_state();

                // process inputs : buttons

                     a.set( state.wButtons &              XINPUT_GAMEPAD_A ? 1.0f : 0.f );
                     b.set( state.wButtons &              XINPUT_GAMEPAD_B ? 1.0f : 0.f );
                     x.set( state.wButtons &              XINPUT_GAMEPAD_X ? 1.0f : 0.f );
                     y.set( state.wButtons &              XINPUT_GAMEPAD_Y ? 1.0f : 0.f );
                  back.set( state.wButtons &           XINPUT_GAMEPAD_BACK ? 1.0f : 0.f );
                 start.set( state.wButtons &          XINPUT_GAMEPAD_START ? 1.0f : 0.f );
                    lb.set( state.wButtons &  XINPUT_GAMEPAD_LEFT_SHOULDER ? 1.0f : 0.f );
                    rb.set( state.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? 1.0f : 0.f );
                lthumb.set( state.wButtons &     XINPUT_GAMEPAD_LEFT_THUMB ? 1.0f : 0.f );
                rthumb.set( state.wButtons &    XINPUT_GAMEPAD_RIGHT_THUMB ? 1.0f : 0.f );

                // process inputs : triggers

                ltrigger.set( state.bLeftTrigger / 255.0 );
                rtrigger.set( state.bRightTrigger / 255.0 );

                // process inputs : digital pad

                float padUp    = ( state.wButtons &    XINPUT_GAMEPAD_DPAD_UP ? 1.0f : 0.f );
                float padDown  = ( state.wButtons &  XINPUT_GAMEPAD_DPAD_DOWN ? 1.0f : 0.f );
                float padLeft  = ( state.wButtons &  XINPUT_GAMEPAD_DPAD_LEFT ? 1.0f : 0.f );
                float padRight = ( state.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? 1.0f : 0.f );
                pad.set( - padLeft + padRight, + padUp - padDown );

                // process inputs : thumb pads

                float thumbLX = state.sThumbLX / ( state.sThumbLX > 0 ? 32767.0 : 32768.0 );
                float thumbLY = state.sThumbLY / ( state.sThumbLY > 0 ? 32767.0 : 32768.0 );
                float thumbRX = state.sThumbRX / ( state.sThumbRX > 0 ? 32767.0 : 32768.0 );
                float thumbRY = state.sThumbRY / ( state.sThumbRY > 0 ? 32767.0 : 32768.0 );
                lpad.set( thumbLX, thumbLY );
                rpad.set( thumbRX, thumbRY );

                // process outputs : rumble

                set_rumble( rumble[0].newest().x, rumble[1].newest().x );

                /*
                // process virtual buttons

                brake.set( pad.bLeftTrigger / 255.0 );

                thrust.set( pad.bRightTrigger / 255.0 );

                float thumbLX = pad.sThumbLX / ( pad.sThumbLX > 0 ? 32767.0 : 32768.0 );
                turn.set( thumbLX );
                */
            }

            /*
            void test()
            {
                if( earphones.isReady() )
                {
                    std::vector< vec2 > soundSample;
                    earphones << soundSample;

                    vec2 soundSample2[500];
                    earphones << 500 << soundSample2;
                }

                if( rumble.isReady() )
                {
                    rumble << vec2( 1, 1 );
                }

                if( axis.isReady() )
                {
                    vec2 current;
                    //axis[0] >> current;

                    if( current.x > 0.5f ) 0;
                    if( current.y > 0.5f ) 0;
                    if( current.magnitude() > 0.5f ) 0;

                    std::vector<vec2> last_4_samples(4);
                    //axis[0] >> 4 >> last_4_samples;
                }

                if( keys.isReady() )
                {
                    //
                    // ...
                }

                if( mic.isReady() )
                {
                    std::vector<vec1> recording( 300 ); //300 muestras
                    mic >> 300 >> recording;
                }

                // encuentra patterns en el buffer de datas y devuelve probabilidad

                //if peak in last 0.10 seconds in cross is at least 80% found then...
                if( hid::peak( axis[0], 0.10f ) > 0.80f )
                {}

                if( hid::sustain( axis[0], 0.25f ) > 0.50f )
                {}

                vec2 hadoken[3] = { vec2(0,1),vec2(1,1),vec2(1,0) };
                if( hid::stroke( axis[0], 0.25f, 3, hadoken ) > 0.85f )
                {}
            }
            */
        };
    }

    namespace windows
    {

        class keyboard //, public hyde::enums::keyboard
        {
            size_t id;

        public:

            // we are implementing keyboard from two sides:
            // - an input serial data (able to send strings to app)
            // - a 105 input button map (so we can query each key status individually)

            const char *const typeof;

            hyde::buttons keymap;
            hyde::serializers serial;

            /* sorry about the long reference memberlist :) */
            hyde::button &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l,
                &m, &n, &o, &p, &q, &r, &s, &t, &u, &v, &w, &x, &y, &z,
                &one, &two, &three, &four, &five, &six, &seven, &eight, &nine, &zero,
                &escape, &backspace, &tab, &enter, &shift, &ctrl, &alt, &space,
                &up, &down, &left, &right, &home, &end, &insert, &del,
                &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12,
                &numpad1, &numpad2, &numpad3, &numpad4, &numpad5,
                &numpad6, &numpad7, &numpad8, &numpad9, &numpad0,
                &add, &subtract, &multiply, &divide, &separator, &decimal;

            hyde::button debug_key;

            hyde::flag is_ready; //is_connected

            keyboard *master;

            keyboard( const size_t &_id )
#if 1
            :
#else
            try :
#endif
                              id(_id),
                  typeof("hyde::windows::keyboard"),
                     keymap( 256 ), serial( 1 ),
                   a( keymap[ hyde::keycode::A ] ),
                   b( keymap[ hyde::keycode::B ] ),
                   c( keymap[ hyde::keycode::C ] ),
                   d( keymap[ hyde::keycode::D ] ),
                   e( keymap[ hyde::keycode::E ] ),
                   f( keymap[ hyde::keycode::F ] ),
                   g( keymap[ hyde::keycode::G ] ),
                   h( keymap[ hyde::keycode::H ] ),
                   i( keymap[ hyde::keycode::I ] ),
                   j( keymap[ hyde::keycode::J ] ),
                   k( keymap[ hyde::keycode::K ] ),
                   l( keymap[ hyde::keycode::L ] ),
                   m( keymap[ hyde::keycode::M ] ),
                   n( keymap[ hyde::keycode::N ] ),
                   o( keymap[ hyde::keycode::O ] ),
                   p( keymap[ hyde::keycode::P ] ),
                   q( keymap[ hyde::keycode::Q ] ),
                   r( keymap[ hyde::keycode::R ] ),
                   s( keymap[ hyde::keycode::S ] ),
                   t( keymap[ hyde::keycode::T ] ),
                   u( keymap[ hyde::keycode::U ] ),
                   v( keymap[ hyde::keycode::V ] ),
                   w( keymap[ hyde::keycode::W ] ),
                   x( keymap[ hyde::keycode::X ] ),
                   y( keymap[ hyde::keycode::Y ] ),
                   z( keymap[ hyde::keycode::Z ] ),
                 one( keymap[ hyde::keycode::ONE ] ),
                 two( keymap[ hyde::keycode::TWO ] ),
               three( keymap[ hyde::keycode::THREE ] ),
                four( keymap[ hyde::keycode::FOUR ] ),
                five( keymap[ hyde::keycode::FIVE ] ),
                 six( keymap[ hyde::keycode::SIX ] ),
               seven( keymap[ hyde::keycode::SEVEN ] ),
               eight( keymap[ hyde::keycode::EIGHT ] ),
                nine( keymap[ hyde::keycode::NINE ] ),
                zero( keymap[ hyde::keycode::ZERO ] ),
              escape( keymap[ hyde::keycode::ESCAPE ] ),
           backspace( keymap[ hyde::keycode::BACKSPACE ] ),
                 tab( keymap[ hyde::keycode::TAB ] ),
               enter( keymap[ hyde::keycode::ENTER ] ),
               shift( keymap[ hyde::keycode::SHIFT ] ),
                ctrl( keymap[ hyde::keycode::CTRL ] ),
                 alt( keymap[ hyde::keycode::ALT ] ),
               space( keymap[ hyde::keycode::SPACE ] ),
                  up( keymap[ hyde::keycode::UP ] ),
                down( keymap[ hyde::keycode::DOWN ] ),
                left( keymap[ hyde::keycode::LEFT ] ),
               right( keymap[ hyde::keycode::RIGHT ] ),
                home( keymap[ hyde::keycode::HOME ] ),
                 end( keymap[ hyde::keycode::END ] ),
              insert( keymap[ hyde::keycode::INSERT ] ),
                 del( keymap[ hyde::keycode::DEL ] ),
                  f1( keymap[ hyde::keycode::F1 ] ),
                  f2( keymap[ hyde::keycode::F2 ] ),
                  f3( keymap[ hyde::keycode::F3 ] ),
                  f4( keymap[ hyde::keycode::F4 ] ),
                  f5( keymap[ hyde::keycode::F5 ] ),
                  f6( keymap[ hyde::keycode::F6 ] ),
                  f7( keymap[ hyde::keycode::F7 ] ),
                  f8( keymap[ hyde::keycode::F8 ] ),
                  f9( keymap[ hyde::keycode::F9 ] ),
                 f10( keymap[ hyde::keycode::F10 ] ),
                 f11( keymap[ hyde::keycode::F11 ] ),
                 f12( keymap[ hyde::keycode::F12 ] ),
             numpad1( keymap[ hyde::keycode::NUMPAD1 ] ),
             numpad2( keymap[ hyde::keycode::NUMPAD2 ] ),
             numpad3( keymap[ hyde::keycode::NUMPAD3 ] ),
             numpad4( keymap[ hyde::keycode::NUMPAD4 ] ),
             numpad5( keymap[ hyde::keycode::NUMPAD5 ] ),
             numpad6( keymap[ hyde::keycode::NUMPAD6 ] ),
             numpad7( keymap[ hyde::keycode::NUMPAD7 ] ),
             numpad8( keymap[ hyde::keycode::NUMPAD8 ] ),
             numpad9( keymap[ hyde::keycode::NUMPAD9 ] ),
             numpad0( keymap[ hyde::keycode::NUMPAD0 ] ),
                 add( keymap[ hyde::keycode::ADD ] ),
            subtract( keymap[ hyde::keycode::SUBTRACT ] ),
            multiply( keymap[ hyde::keycode::MULTIPLY ] ),
              divide( keymap[ hyde::keycode::DIVIDE ] ),
           separator( keymap[ hyde::keycode::SEPARATOR ] ),
             decimal( keymap[ hyde::keycode::DECIMAL ] )
            {
                if( id >= max_devices )
                {
                    std::cerr << "error: device id #" << id << " invalid! (seen at " << typeof << " controller)" << std::endl;
                    assert( false );
                    return;
                }

                // check & increment instance counter
                master = sharing_policy::get_master( *this, id, true );
            }
#if 0
            catch(...)
            {
                throw "";
            }
#endif

            ~keyboard()
            {
                if( master == this )
                {
                    // remove pending keystrokes

                    std::cin.clear();
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max());
                }

                // check & decrement instance counter
                master = sharing_policy::get_master( *this, id, false );
            }

            static const size_t max_devices = 1;

            void clear()
            {
                for( auto &it : keymap )
                    it.clear();

                for( auto &it : serial )
                    it.clear();

                debug_key.clear();
                is_ready.clear();
            }

            void update()
            {
                if( master != this )
                {
                    this->keymap = master->keymap;
                    this->serial = master->serial;
                    this->debug_key = master->debug_key;
                    this->is_ready = master->is_ready;

                    return;
                    /*
            hyde::button &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l,
                &m, &n, &o, &p, &q, &r, &s, &t, &u, &v, &w, &x, &y, &z,
                &one, &two, &three, &four, &five, &six, &seven, &eight, &nine, &zero,
                &escape, &backspace, &tab, &enter, &shift, &ctrl, &space,
                &up, &down, &left, &right, &home, &end, &insert, &del,
                &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12,
                &numpad1, &numpad2, &numpad3, &numpad4, &numpad5,
                &numpad6, &numpad7, &numpad8, &numpad9, &numpad0,
                &add, &subtract, &multiply, &divide, &separator, &decimal;
                */
                }

				if( !GetFocus() )
                {
                    if( GetForegroundWindow() != GetConsoleWindow() )
                    {
                        clear();
                        return;
                    }
                }
                else
                {
                    MSG m;
                    while( PeekMessage( &m, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE ) )
                        ;
                }

                bool is_connected = true;

                // to check: http://molecularmusings.wordpress.com/2011/09/05/properly-handling-keyboard-input/

                /*
                return ( GetFocus() != 0 );

                static HWND myWnd;
                static struct install { install() {
                    HWND prevWnd;
                    myWnd = SetCapture( prevWnd = GetCapture() );
                    //GetWindowThreadProcessId( myWnd, &myProc );
                    ReleaseCapture();
                } } _;
                return GetForegroundWindow() == myWnd;
                */

                is_ready.set( is_connected );

                if( !is_connected )
                    return;

                //GetKeyboardState( (PBYTE)&keymap ) )

                for( size_t i = 0; i < 256; ++i )
                {
                    SHORT key = GetAsyncKeyState( i );
                    keymap[ i ].set( key & 0x8000 ? 0.5f : 0.f );

                    //if( key ) serializer << ch; // y q pasa con el cero?
                }
            }
        };


        class mouse //, public hyde::enums::mouse
        {
            size_t id;

            public:

            enum button_enumeration
            {
                LEFT,
                MIDDLE,
                RIGHT
            };

            enum flag_enumeration
            {
                HOVER,
                CONNECTED,
                HIDDEN,
                CLIPPED,
                CENTERED
            };

            enum axis_enumeration
            {
                WHEEL,
                LOCAL,		// app (dt,dt) center = (0,0)
                GLOBAL,		// desktop (x,y)
                CLIENT,		// app (x,y)
                DESKTOP		// desktop (dt,dt) center = (0,0)
            };

            // como hago la memoization?
            // q es mejor:
            // - leo las entradas, y cada vez que me preguntan un combo compruebo todas las combinaciones?
            //   combo["punch"]? : return (if A[0].hold() && A[1].hold() && A[2].hold() && B[3].tap() ? true : false)
            // - o con cada entrada que leo sigo todas las maquinas de estados vivas, y actualizo valores de retorno
            //   para cuando me consulten?
            //   read pad -> A -> fsm #1 -> step3/13 -> combo["punch"] = false;
            //                    fsm #2 -> step19/19 -> "kick" -> combo["kick"] = true;
            // - hago un map<string,val> cache; y lo limpio/lleno en cada update?

            hyde::flags flags;
            hyde::buttons buttons;
            hyde::coordinates coordinates;

            hyde::button &left, &middle, &right;
            hyde::coordinate &wheel, &local, &global, &client, &desktop;
            hyde::flag &hover, &connected, &hidden, &clipped, &centered;

            mouse *master;

        protected:
            int ix, iy;
            bool check_console_window;

        public:
                 mouse( const size_t &_id, bool check_console_window = false ) :
                    id(_id), ix(0), iy(0),
              flags( 5 ), buttons( 3 ), coordinates( 5 ),
                    left( buttons[ LEFT ] ),
                  middle( buttons[ MIDDLE ] ),
                   right( buttons[ RIGHT ] ),
                   wheel( coordinates[ WHEEL ] ),
                   local( coordinates[ LOCAL ] ),
                  global( coordinates[ GLOBAL ] ),
                  client( coordinates[ CLIENT ]),
                 desktop( coordinates[ DESKTOP ] ),
                   hover( flags[ HOVER ] ),
               connected( flags[ CONNECTED ] ),
                  hidden( flags[ HIDDEN ] ),
                 clipped( flags[ CLIPPED ] ),
                centered( flags[ CENTERED ]),
               typeof( "hyde::windows::mouse" ),
          check_console_window( check_console_window )
            {
                if( id >= max_devices )
                {
                    std::cerr << "error: device id #" << id << " invalid! (seen at " << typeof << " controller)" << std::endl;
                    assert( false );
                    return;
                }
                //
                // caps = hyde::caps( max_devices, max_instances_per_device );

                // check & decrement instance counter
                master = sharing_policy::get_master( *this, id, true );
            }

            ~mouse()
            {
                // check & decrement instance counter
                master = sharing_policy::get_master( *this, id, false );
            }

            static const size_t max_devices = 1;

            const char *const typeof;

            void clear()
            {
                for( auto &it : buttons )
                    it.clear();
                for( auto &it : coordinates )
                    it.clear();
                for( auto &it : flags )
                    it.clear();
            }

            void update()
            {
                if( master != this )
                {
                    this->flags = master->flags;
                    this->buttons = master->buttons;
                    this->coordinates = master->coordinates;
                    return;
                }

#if 1

                if( !GetFocus() )
                {
                    if( GetForegroundWindow() != GetConsoleWindow() )
                    {
                        clear();
                        return;
                    }
                }
                else
                {
                    MSG m;
                    while( PeekMessage( &m, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE ) )
                        ;
                }

                // @todo: replace is_present with a proper/reliable is_present() function

                static HWND parentWnd = 0;
                static bool is_mouse_present = true;
                static HCURSOR default_cursor;
                static HCURSOR hidden_cursor;
                static HICON hicon;

                static struct detection_setup {
                    detection_setup() {
                        HWND prevWnd; parentWnd = SetCapture( prevWnd = GetCapture() ); //ReleaseCapture();
                        is_mouse_present = ShowCursor( true ) >= 0;
                        default_cursor = LoadCursor(0, IDC_ARROW);

                         // make hidden_cursor transparent

                        int w = GetSystemMetrics( SM_CXCURSOR );
                        int h = GetSystemMetrics( SM_CYCURSOR );

                        std::vector<unsigned char> and_mask( w * h, 0xFF );
                        std::vector<unsigned char> xor_mask( w * h, 0x00 );

                        hidden_cursor = CreateCursor(
                          0,
                          1,
                          1,
                          w,
                          h,
                          &and_mask[0],
                          &xor_mask[0]
                        );

                        BYTE ANDmaskIcon[] = {0xFF, 0xFF, 0xFF, 0xFF,  // line 1
                                              0xFF, 0xFF, 0xC3, 0xFF,  // line 2
                                              0xFF, 0xFF, 0x00, 0xFF,  // line 3
                                              0xFF, 0xFE, 0x00, 0x7F,  // line 4

                                              0xFF, 0xFC, 0x00, 0x1F,  // line 5
                                              0xFF, 0xF8, 0x00, 0x0F,  // line 6
                                              0xFF, 0xF8, 0x00, 0x0F,  // line 7
                                              0xFF, 0xF0, 0x00, 0x07,  // line 8

                                              0xFF, 0xF0, 0x00, 0x03,  // line 9
                                              0xFF, 0xE0, 0x00, 0x03,  // line 10
                                              0xFF, 0xE0, 0x00, 0x01,  // line 11
                                              0xFF, 0xE0, 0x00, 0x01,  // line 12

                                              0xFF, 0xF0, 0x00, 0x01,  // line 13
                                              0xFF, 0xF0, 0x00, 0x00,  // line 14
                                              0xFF, 0xF8, 0x00, 0x00,  // line 15
                                              0xFF, 0xFC, 0x00, 0x00,  // line 16

                                              0xFF, 0xFF, 0x00, 0x00,  // line 17
                                              0xFF, 0xFF, 0x80, 0x00,  // line 18
                                              0xFF, 0xFF, 0xE0, 0x00,  // line 19
                                              0xFF, 0xFF, 0xE0, 0x01,  // line 20

                                              0xFF, 0xFF, 0xF0, 0x01,  // line 21
                                              0xFF, 0xFF, 0xF0, 0x01,  // line 22
                                              0xFF, 0xFF, 0xF0, 0x03,  // line 23
                                              0xFF, 0xFF, 0xE0, 0x03,  // line 24

                                              0xFF, 0xFF, 0xE0, 0x07,  // line 25
                                              0xFF, 0xFF, 0xC0, 0x0F,  // line 26
                                              0xFF, 0xFF, 0xC0, 0x0F,  // line 27
                                              0xFF, 0xFF, 0x80, 0x1F,  // line 28

                                              0xFF, 0xFF, 0x00, 0x7F,  // line 29
                                              0xFF, 0xFC, 0x00, 0xFF,  // line 30
                                              0xFF, 0xF8, 0x03, 0xFF,  // line 31
                                              0xFF, 0xFC, 0x3F, 0xFF}; // line 32

                        // Yang-shaped icon XOR mask

                        BYTE XORmaskIcon[] = {0x00, 0x00, 0x00, 0x00,  // line 1
                                              0x00, 0x00, 0x00, 0x00,  // line 2
                                              0x00, 0x00, 0x00, 0x00,  // line 3
                                              0x00, 0x00, 0x00, 0x00,  // line 4

                                              0x00, 0x00, 0x00, 0x00,  // line 5
                                              0x00, 0x00, 0x00, 0x00,  // line 6
                                              0x00, 0x00, 0x00, 0x00,  // line 7
                                              0x00, 0x00, 0x38, 0x00,  // line 8

                                              0x00, 0x00, 0x7C, 0x00,  // line 9
                                              0x00, 0x00, 0x7C, 0x00,  // line 10
                                              0x00, 0x00, 0x7C, 0x00,  // line 11
                                              0x00, 0x00, 0x38, 0x00,  // line 12

                                              0x00, 0x00, 0x00, 0x00,  // line 13
                                              0x00, 0x00, 0x00, 0x00,  // line 14
                                              0x00, 0x00, 0x00, 0x00,  // line 15
                                              0x00, 0x00, 0x00, 0x00,  // line 16

                                              0x00, 0x00, 0x00, 0x00,  // line 17
                                              0x00, 0x00, 0x00, 0x00,  // line 18
                                              0x00, 0x00, 0x00, 0x00,  // line 19
                                              0x00, 0x00, 0x00, 0x00,  // line 20

                                              0x00, 0x00, 0x00, 0x00,  // line 21
                                              0x00, 0x00, 0x00, 0x00,  // line 22
                                              0x00, 0x00, 0x00, 0x00,  // line 23
                                              0x00, 0x00, 0x00, 0x00,  // line 24

                                              0x00, 0x00, 0x00, 0x00,  // line 25
                                              0x00, 0x00, 0x00, 0x00,  // line 26
                                              0x00, 0x00, 0x00, 0x00,  // line 27
                                              0x00, 0x00, 0x00, 0x00,  // line 28

                                              0x00, 0x00, 0x00, 0x00,  // line 29
                                              0x00, 0x00, 0x00, 0x00,  // line 30
                                              0x00, 0x00, 0x00, 0x00,  // line 31
                                              0x00, 0x00, 0x00, 0x00}; // line 32

                        HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

                        hicon  = CreateIcon(hInstance, // handle to app. instance
                                     32,               // icon width
                                     32,               // icon height
                                     1,                // number of XOR planes
                                     1,                // number of bits per pixel
                                     ANDmaskIcon,      // AND mask
                                     XORmaskIcon);     // XOR mask
                    }
                    ~detection_setup() {
                        ReleaseCapture();
                        SetCursor(default_cursor);
                        DestroyCursor(hidden_cursor);
                    }}_;

                connected.set( is_mouse_present ? 0.5f : 0.f );

                if( !is_mouse_present )
                    return;

                // process outputs 1/3

                //SetClassLong( hWnd, GCL_HCURSOR, (LONG)( default_cursor ) );

                // process inputs

                SHORT lkey = GetAsyncKeyState( VK_LBUTTON );
                SHORT mkey = GetAsyncKeyState( VK_MBUTTON );
                SHORT rkey = GetAsyncKeyState( VK_RBUTTON );

                  left.set( lkey & 0x8000 ? 0.5f : 0.f );
                middle.set( mkey & 0x8000 ? 0.5f : 0.f );
                 right.set( rkey & 0x8000 ? 0.5f : 0.f );

                bool is_hover = false;

                CURSORINFO ci;
                ci.cbSize = sizeof(CURSORINFO);

                // disable clipping
                ClipCursor( 0 );

                if( GetCursorInfo( &ci ) )
                {
                    POINT pPoint = ci.ptScreenPos;

                    // set virtual screen coordinates in normalised [-1,+1]x[-1,+1] form whose center is (0,0)
                    {
                        float gx, gy;

                        gx = -1.0f + float(pPoint.x+pPoint.x) / float( GetSystemMetrics( SM_CXVIRTUALSCREEN ) );
                        gy =  1.0f - float(pPoint.y+pPoint.y) / float( GetSystemMetrics( SM_CYVIRTUALSCREEN ) );

                        desktop.set( pPoint.x, pPoint.y );
                        global.set( gx, gy );
                    }

                    HWND hWnd = WindowFromPoint( pPoint );

                    // if [ parent window, console window, or kind-of-subchild ] then...
                    if( hWnd == parentWnd || (hWnd == GetConsoleWindow() && check_console_window) || GetWindowLongPtr(hWnd, GWLP_WNDPROC) )
                    {
                        // point inside any of our windows
                        is_hover = true;

                        // transform point into window->client rect coordinates
                        WINDOWINFO wi;
                        wi.cbSize=sizeof(WINDOWINFO);
                        GetWindowInfo(hWnd, &wi);

                        if( centered.hold() )
                        {
                            int cx = ( wi.rcClient.right - wi.rcClient.left ) / 2 + wi.rcClient.left;
                            int cy = ( wi.rcClient.bottom - wi.rcClient.top ) / 2 + wi.rcClient.top;

                            int dx = pPoint.x - cx;
                            int dy = pPoint.y - cy;

                            SetCursorPos( cx, cy );

                            client.set( ix += dx, iy += dy );
                        }
                        else
                        {
                            pPoint.x -= wi.rcClient.left;
                            pPoint.y -= wi.rcClient.top;

                            client.set( ix = pPoint.x, iy = pPoint.y );
                        }

                        // set coordinate in normalised [-1,+1]x[-1,+1] form whose center is (0,0)
                        {
                            float lx, ly;

                            lx = -1.0f + float(pPoint.x+pPoint.x) / float( wi.rcClient.right - wi.rcClient.left );
                            ly =  1.0f - float(pPoint.y+pPoint.y) / float( wi.rcClient.bottom - wi.rcClient.top + 1 );

                            local.set( lx, ly );
                        }

                        // process outputs 2/3: enable clipping, if required
                        if( clipped.hold() ) ClipCursor( &wi.rcClient );
                    }

                    // process outputs 3/3: hide cursor, if required

                    {
                        bool hide_request = hidden.hold();
                        bool show_request = hidden.idle();

                        // method #1
						if(0)
                        {
                            ShowCursor( TRUE );
                            int counter = ShowCursor( FALSE ); //<0: no mouse installed, or hidden cursor

                            bool visible = (counter >= 0);

                            if( visible )
                            {
                                if( hide_request ) while(counter>=0) counter=ShowCursor(FALSE);
                            }
                            else
                            {
                                if( show_request ) while(counter <0) counter=ShowCursor(TRUE);
                            }
                        }

                        // method #2
						if(0)
                        SetClassLongPtr( hWnd, GCLP_HCURSOR, (LONG)( show_request ? default_cursor : hidden_cursor ) );

						// method #3
						if( hide_request ) while( ShowCursor(FALSE) >= 0 ) ;
						else
						if( show_request ) while( ShowCursor(TRUE) < 0 ) ;
                    }

                    // stupid feature:

                    SetClassLongPtr( hWnd, GCLP_HICON, (LONG)(hicon) );
                }

                hover.set( is_hover ? 0.5f : 0.f );

#endif




                static struct once {
                    once() {
                        const int num_mice = ManyMouse_Init();
                        driver = ManyMouse_DriverName();
                        for( int i = 0; i < num_mice; ++i )
                        {
                            drivers.push_back( ManyMouse_DeviceName(i) );
                        }
                    }
                    ~once() {
                        ManyMouse_Quit();
                    }
                    const char *driver;
                    std::vector<const char *> drivers;
                } lib;

                    ManyMouseEvent event;
                    static float dx = 0, dy = 0;

                    if( ManyMouse_PollEvent(&event) && event.device == 0 )
                    {
                        if (event.type == MANYMOUSE_EVENT_SCROLL )
                        {
                            if( event.item == 0 )
                            {
                                dy += ( event.value > 0 ? 1 : -1 ) * 0.100f;
                            }
                            else
                            {
                                dx += ( event.value > 0 ? 1 : -1 ) * 0.100f;
                            }
                        }

#if 0
                        else if (event.type == MANYMOUSE_EVENT_RELMOTION )
                        {
                            printf("Mouse #%u relative motion %s %d\n", event.device,
                                    event.item == 0 ? "X" : "Y", event.value);
                        }

                        else if (event.type == MANYMOUSE_EVENT_ABSMOTION )
                        {
                            printf("Mouse #%u absolute motion %s %d\n", event.device,
                                    event.item == 0 ? "X" : "Y", event.value);
                        }

                        else if (event.type == MANYMOUSE_EVENT_BUTTON )
                        {
                            if( event.item == 0 ) buttons[ LEFT ].set( event.value ? 0.5f : 0.f );
                            else
                            if( event.item == 1 ) buttons[ MIDDLE ].set( event.value ? 0.5f : 0.f );
                            else
                            if( event.item == 2 ) buttons[ RIGHT ].set( event.value ? 0.5f : 0.f );
                        }

                        else if (event.type == MANYMOUSE_EVENT_DISCONNECT )
                        {
                            connected.set( 0.f );
                        }

                        else
                        {
                            // unhandled event.type
                        }
#endif

                    }

                    wheel.set( dx, dy );
            }
        };
    }
}


#endif
