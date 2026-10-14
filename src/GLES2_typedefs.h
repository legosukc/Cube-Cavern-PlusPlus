#pragma once

// for the PS Vita, since PVR doesn't define types

#include "define.h"

#include <GLES2/gl2.h>

typedef void(GL_APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void(GL_APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(GL_APIENTRYP PFNGLBINDATTRIBLOCATIONPROC)(GLuint program,
                                                       GLuint index,
                                                       const GLchar* name);
typedef void(GL_APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void(GL_APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum target,
                                                    GLuint framebuffer);
typedef void(GL_APIENTRYP PFNGLBINDRENDERBUFFERPROC)(GLenum target,
                                                     GLuint renderbuffer);
typedef void(GL_APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void(GL_APIENTRYP PFNGLBLENDCOLORPROC)(GLfloat red,
                                               GLfloat green,
                                               GLfloat blue,
                                               GLfloat alpha);
typedef void(GL_APIENTRYP PFNGLBLENDEQUATIONPROC)(GLenum mode);
typedef void(GL_APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB,
                                                          GLenum modeAlpha);
typedef void(GL_APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void(GL_APIENTRYP PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB,
                                                      GLenum dfactorRGB,
                                                      GLenum sfactorAlpha,
                                                      GLenum dfactorAlpha);
typedef void(GL_APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target,
                                               GLsizeiptr size,
                                               const void* data,
                                               GLenum usage);
typedef void(GL_APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target,
                                                  GLintptr offset,
                                                  GLsizeiptr size,
                                                  const void* data);
typedef GLenum(GL_APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void(GL_APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
typedef void(GL_APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red,
                                               GLfloat green,
                                               GLfloat blue,
                                               GLfloat alpha);
typedef void(GL_APIENTRYP PFNGLCLEARDEPTHFPROC)(GLfloat d);
typedef void(GL_APIENTRYP PFNGLCLEARSTENCILPROC)(GLint s);
typedef void(GL_APIENTRYP PFNGLCOLORMASKPROC)(GLboolean red,
                                              GLboolean green,
                                              GLboolean blue,
                                              GLboolean alpha);
typedef void(GL_APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void(GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target,
                                                         GLint level,
                                                         GLenum internalformat,
                                                         GLsizei width,
                                                         GLsizei height,
                                                         GLint border,
                                                         GLsizei imageSize,
                                                         const void* data);
typedef void(GL_APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target,
                                                            GLint level,
                                                            GLint xoffset,
                                                            GLint yoffset,
                                                            GLsizei width,
                                                            GLsizei height,
                                                            GLenum format,
                                                            GLsizei imageSize,
                                                            const void* data);
typedef void(GL_APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target,
                                                   GLint level,
                                                   GLenum internalformat,
                                                   GLint x,
                                                   GLint y,
                                                   GLsizei width,
                                                   GLsizei height,
                                                   GLint border);
typedef void(GL_APIENTRYP PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target,
                                                      GLint level,
                                                      GLint xoffset,
                                                      GLint yoffset,
                                                      GLint x,
                                                      GLint y,
                                                      GLsizei width,
                                                      GLsizei height);
typedef GLuint(GL_APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef GLuint(GL_APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void(GL_APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
typedef void(GL_APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n,
                                                  const GLuint* buffers);
typedef void(GL_APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(
    GLsizei n,
    const GLuint* framebuffers);
typedef void(GL_APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void(GL_APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(
    GLsizei n,
    const GLuint* renderbuffers);
typedef void(GL_APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
typedef void(GL_APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n,
                                                   const GLuint* textures);
typedef void(GL_APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void(GL_APIENTRYP PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void(GL_APIENTRYP PFNGLDEPTHRANGEFPROC)(GLfloat n, GLfloat f);
typedef void(GL_APIENTRYP PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(GL_APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
typedef void(GL_APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(GL_APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode,
                                               GLint first,
                                               GLsizei count);
typedef void(GL_APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode,
                                                 GLsizei count,
                                                 GLenum type,
                                                 const void* indices);
typedef void(GL_APIENTRYP PFNGLENABLEPROC)(GLenum cap);
typedef void(GL_APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(GL_APIENTRYP PFNGLFINISHPROC)(void);
typedef void(GL_APIENTRYP PFNGLFLUSHPROC)(void);
typedef void(GL_APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC)(
    GLenum target,
    GLenum attachment,
    GLenum renderbuffertarget,
    GLuint renderbuffer);
typedef void(GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target,
                                                         GLenum attachment,
                                                         GLenum textarget,
                                                         GLuint texture,
                                                         GLint level);
typedef void(GL_APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
typedef void(GL_APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void(GL_APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void(GL_APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei n,
                                                    GLuint* framebuffers);
typedef void(GL_APIENTRYP PFNGLGENRENDERBUFFERSPROC)(GLsizei n,
                                                     GLuint* renderbuffers);
typedef void(GL_APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint* textures);
typedef void(GL_APIENTRYP PFNGLGETACTIVEATTRIBPROC)(GLuint program,
                                                    GLuint index,
                                                    GLsizei bufSize,
                                                    GLsizei* length,
                                                    GLint* size,
                                                    GLenum* type,
                                                    GLchar* name);
typedef void(GL_APIENTRYP PFNGLGETACTIVEUNIFORMPROC)(GLuint program,
                                                     GLuint index,
                                                     GLsizei bufSize,
                                                     GLsizei* length,
                                                     GLint* size,
                                                     GLenum* type,
                                                     GLchar* name);
typedef void(GL_APIENTRYP PFNGLGETATTACHEDSHADERSPROC)(GLuint program,
                                                       GLsizei maxCount,
                                                       GLsizei* count,
                                                       GLuint* shaders);
typedef GLint(GL_APIENTRYP PFNGLGETATTRIBLOCATIONPROC)(GLuint program,
                                                       const GLchar* name);
typedef void(GL_APIENTRYP PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean* data);
typedef void(GL_APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target,
                                                         GLenum pname,
                                                         GLint* params);
typedef GLenum(GL_APIENTRYP PFNGLGETERRORPROC)(void);
typedef void(GL_APIENTRYP PFNGLGETFLOATVPROC)(GLenum pname, GLfloat* data);
typedef void(GL_APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(
    GLenum target,
    GLenum attachment,
    GLenum pname,
    GLint* params);
typedef void(GL_APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint* data);
typedef void(GL_APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program,
                                                 GLenum pname,
                                                 GLint* params);
typedef void(GL_APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program,
                                                      GLsizei bufSize,
                                                      GLsizei* length,
                                                      GLchar* infoLog);
typedef void(GL_APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC)(GLenum target,
                                                               GLenum pname,
                                                               GLint* params);
typedef void(GL_APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader,
                                                GLenum pname,
                                                GLint* params);
typedef void(GL_APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader,
                                                     GLsizei bufSize,
                                                     GLsizei* length,
                                                     GLchar* infoLog);
typedef void(GL_APIENTRYP PFNGLGETSHADERPRECISIONFORMATPROC)(
    GLenum shadertype,
    GLenum precisiontype,
    GLint* range,
    GLint* precision);
typedef void(GL_APIENTRYP PFNGLGETSHADERSOURCEPROC)(GLuint shader,
                                                    GLsizei bufSize,
                                                    GLsizei* length,
                                                    GLchar* source);
typedef const GLubyte*(GL_APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
typedef void(GL_APIENTRYP PFNGLGETTEXPARAMETERFVPROC)(GLenum target,
                                                      GLenum pname,
                                                      GLfloat* params);
typedef void(GL_APIENTRYP PFNGLGETTEXPARAMETERIVPROC)(GLenum target,
                                                      GLenum pname,
                                                      GLint* params);
typedef void(GL_APIENTRYP PFNGLGETUNIFORMFVPROC)(GLuint program,
                                                 GLint location,
                                                 GLfloat* params);
typedef void(GL_APIENTRYP PFNGLGETUNIFORMIVPROC)(GLuint program,
                                                 GLint location,
                                                 GLint* params);
typedef GLint(GL_APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program,
                                                        const GLchar* name);
typedef void(GL_APIENTRYP PFNGLGETVERTEXATTRIBFVPROC)(GLuint index,
                                                      GLenum pname,
                                                      GLfloat* params);
typedef void(GL_APIENTRYP PFNGLGETVERTEXATTRIBIVPROC)(GLuint index,
                                                      GLenum pname,
                                                      GLint* params);
typedef void(GL_APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC)(GLuint index,
                                                            GLenum pname,
                                                            void** pointer);
typedef void(GL_APIENTRYP PFNGLHINTPROC)(GLenum target, GLenum mode);
typedef GLboolean(GL_APIENTRYP PFNGLISBUFFERPROC)(GLuint buffer);
typedef GLboolean(GL_APIENTRYP PFNGLISENABLEDPROC)(GLenum cap);
typedef GLboolean(GL_APIENTRYP PFNGLISFRAMEBUFFERPROC)(GLuint framebuffer);
typedef GLboolean(GL_APIENTRYP PFNGLISPROGRAMPROC)(GLuint program);
typedef GLboolean(GL_APIENTRYP PFNGLISRENDERBUFFERPROC)(GLuint renderbuffer);
typedef GLboolean(GL_APIENTRYP PFNGLISSHADERPROC)(GLuint shader);
typedef GLboolean(GL_APIENTRYP PFNGLISTEXTUREPROC)(GLuint texture);
typedef void(GL_APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat width);
typedef void(GL_APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void(GL_APIENTRYP PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void(GL_APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat factor,
                                                  GLfloat units);
typedef void(GL_APIENTRYP PFNGLREADPIXELSPROC)(GLint x,
                                               GLint y,
                                               GLsizei width,
                                               GLsizei height,
                                               GLenum format,
                                               GLenum type,
                                               void* pixels);
typedef void(GL_APIENTRYP PFNGLRELEASESHADERCOMPILERPROC)(void);
typedef void(GL_APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target,
                                                        GLenum internalformat,
                                                        GLsizei width,
                                                        GLsizei height);
typedef void(GL_APIENTRYP PFNGLSAMPLECOVERAGEPROC)(GLfloat value,
                                                   GLboolean invert);
typedef void(GL_APIENTRYP PFNGLSCISSORPROC)(GLint x,
                                            GLint y,
                                            GLsizei width,
                                            GLsizei height);
typedef void(GL_APIENTRYP PFNGLSHADERBINARYPROC)(GLsizei count,
                                                 const GLuint* shaders,
                                                 GLenum binaryFormat,
                                                 const void* binary,
                                                 GLsizei length);
typedef void(GL_APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader,
                                                 GLsizei count,
                                                 const GLchar* const* string,
                                                 const GLint* length);
typedef void(GL_APIENTRYP PFNGLSTENCILFUNCPROC)(GLenum func,
                                                GLint ref,
                                                GLuint mask);
typedef void(GL_APIENTRYP PFNGLSTENCILFUNCSEPARATEPROC)(GLenum face,
                                                        GLenum func,
                                                        GLint ref,
                                                        GLuint mask);
typedef void(GL_APIENTRYP PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void(GL_APIENTRYP PFNGLSTENCILMASKSEPARATEPROC)(GLenum face,
                                                        GLuint mask);
typedef void(GL_APIENTRYP PFNGLSTENCILOPPROC)(GLenum fail,
                                              GLenum zfail,
                                              GLenum zpass);
typedef void(GL_APIENTRYP PFNGLSTENCILOPSEPARATEPROC)(GLenum face,
                                                      GLenum sfail,
                                                      GLenum dpfail,
                                                      GLenum dppass);
typedef void(GL_APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target,
                                               GLint level,
                                               GLint internalformat,
                                               GLsizei width,
                                               GLsizei height,
                                               GLint border,
                                               GLenum format,
                                               GLenum type,
                                               const void* pixels);
typedef void(GL_APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target,
                                                  GLenum pname,
                                                  GLfloat param);
typedef void(GL_APIENTRYP PFNGLTEXPARAMETERFVPROC)(GLenum target,
                                                   GLenum pname,
                                                   const GLfloat* params);
typedef void(GL_APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target,
                                                  GLenum pname,
                                                  GLint param);
typedef void(GL_APIENTRYP PFNGLTEXPARAMETERIVPROC)(GLenum target,
                                                   GLenum pname,
                                                   const GLint* params);
typedef void(GL_APIENTRYP PFNGLTEXSUBIMAGE2DPROC)(GLenum target,
                                                  GLint level,
                                                  GLint xoffset,
                                                  GLint yoffset,
                                                  GLsizei width,
                                                  GLsizei height,
                                                  GLenum format,
                                                  GLenum type,
                                                  const void* pixels);
typedef void(GL_APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void(GL_APIENTRYP PFNGLUNIFORM1FVPROC)(GLint location,
                                               GLsizei count,
                                               const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void(GL_APIENTRYP PFNGLUNIFORM1IVPROC)(GLint location,
                                               GLsizei count,
                                               const GLint* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM2FPROC)(GLint location,
                                              GLfloat v0,
                                              GLfloat v1);
typedef void(GL_APIENTRYP PFNGLUNIFORM2FVPROC)(GLint location,
                                               GLsizei count,
                                               const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM2IPROC)(GLint location,
                                              GLint v0,
                                              GLint v1);
typedef void(GL_APIENTRYP PFNGLUNIFORM2IVPROC)(GLint location,
                                               GLsizei count,
                                               const GLint* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM3FPROC)(GLint location,
                                              GLfloat v0,
                                              GLfloat v1,
                                              GLfloat v2);
typedef void(GL_APIENTRYP PFNGLUNIFORM3FVPROC)(GLint location,
                                               GLsizei count,
                                               const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM3IPROC)(GLint location,
                                              GLint v0,
                                              GLint v1,
                                              GLint v2);
typedef void(GL_APIENTRYP PFNGLUNIFORM3IVPROC)(GLint location,
                                               GLsizei count,
                                               const GLint* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM4FPROC)(GLint location,
                                              GLfloat v0,
                                              GLfloat v1,
                                              GLfloat v2,
                                              GLfloat v3);
typedef void(GL_APIENTRYP PFNGLUNIFORM4FVPROC)(GLint location,
                                               GLsizei count,
                                               const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUNIFORM4IPROC)(GLint location,
                                              GLint v0,
                                              GLint v1,
                                              GLint v2,
                                              GLint v3);
typedef void(GL_APIENTRYP PFNGLUNIFORM4IVPROC)(GLint location,
                                               GLsizei count,
                                               const GLint* value);
typedef void(GL_APIENTRYP PFNGLUNIFORMMATRIX2FVPROC)(GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)(GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location,
                                                     GLsizei count,
                                                     GLboolean transpose,
                                                     const GLfloat* value);
typedef void(GL_APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void(GL_APIENTRYP PFNGLVALIDATEPROGRAMPROC)(GLuint program);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB1FPROC)(GLuint index, GLfloat x);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB1FVPROC)(GLuint index,
                                                    const GLfloat* v);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB2FPROC)(GLuint index,
                                                   GLfloat x,
                                                   GLfloat y);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB2FVPROC)(GLuint index,
                                                    const GLfloat* v);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB3FPROC)(GLuint index,
                                                   GLfloat x,
                                                   GLfloat y,
                                                   GLfloat z);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB3FVPROC)(GLuint index,
                                                    const GLfloat* v);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB4FPROC)(GLuint index,
                                                   GLfloat x,
                                                   GLfloat y,
                                                   GLfloat z,
                                                   GLfloat w);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIB4FVPROC)(GLuint index,
                                                    const GLfloat* v);
typedef void(GL_APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index,
                                                        GLint size,
                                                        GLenum type,
                                                        GLboolean normalized,
                                                        GLsizei stride,
                                                        const void* pointer);

typedef void(GL_APIENTRYP PFNGLVIEWPORTPROC)(GLint x,
                                          GLint y,
                                          GLsizei width,
                                          GLsizei height);