// © Copyright 2010 - 2015 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifndef _uitextlayer_h
#define _uitextlayer_h

#include "UI/renderlayer.h"
#include "UI/character.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextLine;
        class TextCursor;
        class FontData;
        class Glyph;
        class Sprite;
        class Character;
        class MarkupParser;
        class TextToken;
        class TokenString;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for render layers that render text.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TextLayer : public RenderLayer
        {
        public:
            /// @brief Basic container type for the storage of @ref Character instances by this class.
            typedef std::list<Character*>                           CharacterContainer;
            /// @brief Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::iterator                    CharacterIterator;
            /// @brief Const Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::const_iterator              ConstCharacterIterator;
            /// @brief Reverse Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::reverse_iterator            ReverseCharacterIterator;
            /// @brief Const Reverse Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::const_reverse_iterator      ConstReverseCharacterIterator;
            /// @brief An std::pair type storing two character iterators, usually used to express a range.
            typedef std::pair<CharacterIterator,CharacterIterator>  CharacterIteratorPair;
            /// @brief Basic container type for the storate of @ref TextLine instances by this class.
            typedef std::vector<TextLine*>                          TextLineContainer;
            /// @brief Iterator type for @ref TextLine instances stored by this class.
            typedef TextLineContainer::iterator                     TextLineIterator;
            /// @brief Const Iterator type for @ref TextLine instances stored by this class.
            typedef TextLineContainer::const_iterator               ConstTextLineIterator;
            /// @brief An std::pair type used as a return for index-offset conversions.
            typedef std::pair<Boole,Integer>                        CharIndexPair;
            /// @brief An std::pair type used as a return for index-offset conversions.
            typedef std::pair<Boole,Vector2>                        CharOffsetPair;

            /// @enum AutoScaleMode
            /// @brief An enum used to describe how the text generated by this layer will be automatically scaled.
            enum ScalingMode
            {
                SM_NoAutoScaling        = 0,   ///< There is no auto-scaling applied to text in this layer.  Any scaling has to be done explicitly.
                SM_ScreenRelative       = 1,   ///< The provided scaler will be treated as relative to the Screen Y size.
                SM_ParentRelative       = 2,   ///< The provided scaler will be treated as relative to the parent widget Y size.
                SM_LayerRelative        = 3    ///< The provided scaler will be treated as relative to this layers Y size after layer scaling.
            };
        protected:
            /// @internal
            /// @brief Container with all this layers TextLines.
            TextLineContainer TextLines;
            /// @internal
            /// @brief Container with all this layers Characters.
            CharacterContainer Characters;
            /// @internal
            /// @brief Default set of traits all characters generated inside this layer are to have.
            CharacterTraits DefaultCharTraits;
            /// @internal
            /// @brief The colour of the highlight when this layer belongs to a focused widget.
            ColourValue ActiveHLColour;
            /// @internal
            /// @brief The colour of the highlight when this layer belongs to a widget not being focused.
            ColourValue InactiveHLColour;
            /// @internal
            /// @brief The scaling to apply to all characters in this layer.
            Vector2 ManualCharScaling;
            /// @internal
            /// @brief The parser to use when converting raw strings to renderable characters.
            MarkupParser* MUParser;
            /// @internal
            /// @brief The cursor to be used for insert and manipulation operations.
            TextCursor* Cursor;
            /// @internal
            /// @brief The actual text parsed into tokens used for generating characters.
            TokenString* TextTokens;
            /// @internal
            /// @brief The index of the character at the start of the highlight sequence.
            Integer HighlightStart;
            /// @internal
            /// @brief The index of the character at the end of the highlight sequence.
            Integer HighlightEnd;
            /// @internal
            /// @brief The maximum width of textlines generated by this layer in pixels.
            Real MaxLineWidth;
            /// @internal
            /// @brief The auto-scaling height the text is to be in relative units.
            Real AutoCharScaling;
            /// @internal
            /// @brief The auto-scaling mode that is to be used on text generated by this textlayer.
            ScalingMode AutoCharScalingMode;
            /// @internal
            /// @brief The order text will have in TextLines.
            UI::TextOrdering HorizontalOrder;
            /// @internal
            /// @brief The alignment TextLines will have.
            UI::LinearAlignment VerticalAlign;

            /// @copydoc SimpleRenderer::RedrawImpl(Boole Force)
            virtual void RedrawImpl(Boole Force);
            /// @internal
            /// @brief Recalculates the offset for every text line in this layer.
            virtual void RecalculateOffsets();
            /// @internal
            /// @brief Clears and then places characters belonging to this layer in the appropriate text lines.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            virtual void PopulateTextLinesImpl(const Real MaxWidth) = 0;
            /// @internal
            /// @brief Gets the index of the character at the specified offset position.
            virtual CharIndexPair GetIndexAtOffsetImpl(const Vector2& Position) = 0;
            /// @internal
            /// @brief Gets the position of the character at the specified index.
            virtual CharOffsetPair GetOffsetAtIndexImpl(const Integer Index) = 0;
        //public:
            /// @brief No-Font constructor.
            /// @param ParentRenderable The renderable that created this layer.
            TextLayer(QuadRenderable* ParentRenderable);
            /// @brief Class constructor.
            /// @param FontName The name of the font to use for this layer.
            /// @param ParentRenderable The renderable that created this layer.
            TextLayer(const String& FontName, QuadRenderable* ParentRenderable);
            /// @brief Text-Scaling constructor.
            /// @note This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @param ParentRenderable The renderable that created this layer.
            TextLayer(const Real& LineHeight, QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~TextLayer();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Parser Methods

            /// @brief Sets the MarkupParser to be used by this TextLayer.
            /// @note Passing in NULL will be ignored.
            /// @param Parser A pointer to the MarkupParser to be used by this TextLayer.
            /// @return Returns true if the new MarkupParser was successfully set, false otherwise.
            virtual Boole SetMarkupParser(MarkupParser* Parser);
            /// @brief Sets the MarkupParser to be used by this TextLayer via it's registered name.
            /// @remarks This method will check the UIManager to see if the named MarkupParser exists.  If it does not then it will set NULL.
            /// @param ParserName The name of the registered parser to retrieve and set for this TextLayer.
            /// @return Returns true if the new MarkupParser was successfully set, false otherwise.
            virtual Boole SetMarkupParser(const String& ParserName);
            /// @brief Gets the MarkupParser being used by this TextLayer.
            /// @return Returns a pointer to the MarkupParser currently being used by this TextLayer.
            virtual MarkupParser* GetMarkupParser() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the combined height of all the text lines in this layer.
            /// @return Returns a Real representing the pixel height of all the textlines in this layer.
            virtual Real GetTotalHeight() const;

            /// @brief Gets the maximum width of text lines in this layer.
            /// @return Returns a Real representing the maximum pixel width of text lines generated in this layer.
            virtual Real GetMaxLineWidth() const;
            /// @brief Gets the height in pixels this layer is configured to render it's text.
            /// @return Returns a Real representing the pixel height of text lines in this layer that is desired.
            virtual Real GetDesiredLineHeight() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Offset - Index Conversion Methods

            /// @brief Gets the index of the character at the specified offset position.
            /// @param Offset The offset position of the character to get the index of.
            /// @return Returns a CharIndexPair where the first value is a Boole which will be false if
            /// the position is invalid for whatever reason.  The second value will be in the index.  If
            /// the second value is -1 then the position is to the right of the last character.
            virtual CharIndexPair GetIndexAtOffset(const Vector2& Offset);
            /// @brief Gets the offset position of the character at the provided index.
            /// @param Index The index of the character position to retrieve.
            /// @return Returns a CharOffsetPair where the first value is a Boole which will be false if the
            /// index is invalid for any reason, and the second value is the offset position of the
            /// character at the specified index if the first value is true.
            virtual CharOffsetPair GetOffsetAtIndex(const Integer Index);

            ///////////////////////////////////////////////////////////////////////////////
            // Text Methods

            /// @brief Sets the text displayed within this layer.
            /// @param Text The text to be displayed.
            virtual void SetText(const String& Text);
            /// @brief Gets the text displayed within this layer.
            /// @return Returns the text being displayed.
            virtual String GetText() const;
            /// @brief Sets the default colour of the text being rendered by this layer.
            /// @param Colour The colour to apply to the text of this layer.
            virtual void SetTextColour(const ColourValue& Colour);
            /// @brief Gets the default colour of the tect being rendered by this layer.
            /// @return Returns a const ColourValue reference containing the colour used to render this layer's text.
            virtual const ColourValue& GetTextColour() const;

            /// @brief Sets the scaling to be applied to the text being rendered.
            /// @remarks Manual scaling is applied after any autoscaling settings are applied.
            /// @param Scale A Vector2 representing the scale on both dimensions to be applied.  <1.0 means smaller, >1.0 means larger.
            virtual void SetManualTextScale(const Vector2& Scale);
            /// @brief Gets the scaling currently being applied to the rendered text.
            /// @return Returns a Vector2 representing the scale applied to the text in this layer.  <1.0 means smaller, >1.0 means larger.
            virtual const Vector2& GetManualTextScale() const;
            /// @brief Sets the mode and scaler of auto-scaling applied to the text generated by this textlayer.
            /// @remarks There are a lot of considerations to be made with auto-scaling.  Auto-scaling will adjust the size of the provided font
            /// automatically based on the scaling mode that is set, which really decides what the relative scalar provided for auto-scaling is
            /// relative to.  If auto-scaling is relative to the screen, then the text will only change in size when the screen does.  Parent and
            /// layer relative scaling are very similiar since layers get their dimensions from the parent widget they belong to.  The difference
            /// comes from scaling applied to the layer itself.  Parent relative scaling ignores the layer scaling, while layer relative factors
            /// it in.  @n @n
            /// In addition to the relative component to the scaling, manual scaling will also be applied after the auto-scaling is.  Auto-scaling
            /// will be completely ignored if the custom size on a character is set.
            /// @param Mode A ScalingMode enum value describing what the provided scaler will be relative to.
            /// @param Scalar The relative value that will be used to determine the needed scaling to be applied to text in this layer.
            virtual void SetAutoTextScale(const TextLayer::ScalingMode Mode, const Real Scalar);
            /// @brief Gets the automatic scaling mode being used by this textlayer.
            /// @return Returns a ScalingMode enum value describing what the relative scaler is relative to, if auto-scaling is enabled at all.
            virtual TextLayer::ScalingMode GetAutoTextScalingMode() const;
            /// @brief Gets the relative scalar being used to automatically scale text generated by this layer.
            /// @return Returns a Real representing the relative Y size to use when detmining how to uniformly scale text in this layer.
            virtual Real GetAutoTextScalar() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Font Methods

            /// @brief Sets the default font to be used with this layer.
            /// @param NewFont The default font to use for this layer.
            virtual void SetDefaultFont(FontData* NewFont);
            /// @brief Sets the default font to be used with this layer.
            /// @details The font name is defined in your mta file.  This class can change which
            /// glyph is uses with it's markup language.  This simply defines which to use when one isn't
            /// specified.
            /// @param FontName The name of the font to use for this layer.
            virtual void SetDefaultFont(const String& FontName);
            /// @brief Sets the default font to be used with this layer.
            /// @details The font is defined in your mta file.  This class can change which
            /// glyph is uses with it's markup language.  This simply defines which to use when one isn't
            /// specified.
            /// @param FontName The name of the font to use for this layer.
            /// @param Atlas The different atlas to get the glyphdata from.
            virtual void SetDefaultFont(const String& FontName, const String& Atlas);
            /// @brief Gets the default font in use by this layer.
            /// @details The font is defined in your mta file.
            /// @return Returns a pointer to the font in use by this layer.
            virtual FontData* GetDefaultFont();

            ///////////////////////////////////////////////////////////////////////////////
            // Highlight Methods

            /// @brief Sets the colour of the highlight when the quad is being focused.
            /// @note Being focused is a property of Widgets, which the parent quad may not be.
            /// In general highlighting in a non-Widget is likely or recommended.
            /// @param Colour The highlight colour while being focused.
            virtual void SetActiveHighlightBackgroundColour(const ColourValue& Colour);
            /// @brief Gets the colour of the highlight when the quad is being focused.
            /// @return Returns a const ColourValue reference containing the colour used to render highlights while the parent quad is focused.
            virtual const ColourValue& GetActiveHighlightBackgroundColour() const;
            /// @brief Sets the colour of the highlight when the quad is not focused.
            /// @note Being focused is a property of Widgets, which the parent quad may not be.
            /// In general highlighting in a non-Widget is likely or recommended.
            /// @param Colour The highlight colour while
            virtual void SetInactiveHighlightBackgroundColour(const ColourValue& Colour);
            /// @brief Gets the colour of the highlight when the quad is being focused.
            /// @return Returns a const ColourValue reference containing the colour used to render highlights while the parent quad is not focused.
            virtual const ColourValue& GetInactiveHighlightBackgroundColour() const;

            /// @brief Highlights all characters in this layer.
            virtual void Highlight();
            /// @brief Highlights the character at the specified index.
            /// @param Index The index of the character to highlight.
            virtual void Highlight(const Integer Index);
            /// @brief Highlights all characters in a provided range.
            /// @param StartIndex The index of the first character in the range to highlight.
            /// @param EndIndex The index of the last character in the range to highlight.
            virtual void Highlight(const Integer StartIndex, const Integer EndIndex);
            /// @brief Gets the index of the first character that is highlighted in this layer.
            /// @return Returns the index of the first character in the highlighted range, or -1 if nothing is highlighted.
            virtual Integer GetHighlightStart() const;
            /// @brief Gets the index of this last character that is highlighted in this layer.
            /// @return Returns the index of the last character in the highlighted range, or -1 if nothing is highlighted.
            virtual Integer GetHighlightEnd() const;
            /// @brief Clears all the highlights in this layer.
            virtual void ClearHighlights();

            ///////////////////////////////////////////////////////////////////////////////
            // Ordering Methods

            /// @brief Sets the ordering for characters in this layer.
            /// @remarks The Text Line Order determines if characters are ordered left to right, or right to left.
            /// @param Order The direction of advancement for characters on the horizontal axis.
            virtual void SetTextOrder(const UI::TextOrdering Order);
            /// @brief Gets the currently set direction of advancement for characters on the horizontal axis created by this layer.
            /// @return Returns the direction of advancement for characters on the horizontal axis.
            virtual UI::TextOrdering GetTextOrder() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Cursor Methods

            /// @brief Enables (or disables) the cursor for use in this layer.
            /// @note The default state for the Text Cursor is disabled.
            /// @param Enable True to enable the cursor and make it available for manipulation/rendering, false to disable it.
            virtual void SetCursorEnabled(Boole Enable);
            /// @brief Gets whether or not the Text Cursor is enabled.
            /// @return Returns true if the cursor is being rendered, false otherwise.
            virtual Boole GetCursorEnabled() const;
            /// @brief Gets the TextCursor in use by this layer.
            /// @return Returns a pointer to this layers TextCursor, or NULL if it is disabled.
            virtual TextCursor* GetCursor() const;

            ///////////////////////////////////////////////////////////////////////////////
            // TextLine Methods

            /// @brief Creates a new TextLine.
            /// @note One textline will be auto-created when a text layer is constructed.  It's default horizontal alignment
            /// will be to the left.  Additional text lines will be given the alignment of the last text line in the layers
            /// text line container.
            /// @return Returns a pointer to the created TextLine.
            virtual TextLine* CreateTextLine();
            /// @brief Gets the TextLine at the specified offset position.
            /// @param Offset The offset from the top edge of the parent layer.
            /// @return Returns a pointer to the TextLine at the requested offset, or NULL if none exists.
            virtual TextLine* GetTextLineAtOffset(const Real& Offset);
            /// @brief Gets the number of TextLines this layer contains.
            /// @return Returns a Whole representing the number of lines of text this layer has.
            virtual Whole GetNumTextLines() const;

            /// @brief Populates text lines in this layer with parsed characters.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            virtual void PopulateTextLines(const Real MaxWidth);
            /// @brief Removes all characters from all TextLines belonging to this layer.
            virtual void ClearAllTextLines();
            /// @brief Destroys all TextLines in this layer.
            virtual void DestroyAllTextLines();

            /// @brief Sets the horizontal alignment of every textline in this layer.
            /// @param Align A LinearAlignment enum value indicating how each text line in this layer should align it's text.
            virtual void SetTextLineHorizontalAlignment(const UI::LinearAlignment Align);
            /// @brief Sets the alignment used to determine the start position of the textlines in this layer.
            /// @note TextLines can easily run out of room in a given layer.  If the combined vertical height of the TextLines
            /// in this layer exceed the size given for rendering then this setting is ignored, and text will render from the top.
            /// @param Align The alignment to use.
            virtual void SetTextLineVerticalAlignment(const UI::LinearAlignment Align);
            /// @brief Gets the current set alignment for positioning textlines in this layer.
            /// @return Returns the current TextLine position alignment.
            virtual UI::LinearAlignment GetTextLineVerticalAlignment() const;

            /// @brief Gets an iterator to the first TextLine.
            /// @return Returns an iterator to the first TextLine being stored by this TextLayer.
            virtual TextLineIterator BeginTextLine();
            /// @brief Gets an iterator to one passed the last TextLine.
            /// @return Returns an iterator to one passed the last TextLine being stored by this TextLayer.
            virtual TextLineIterator EndTextLine();
            /// @brief Gets a const iterator to the first TextLine.
            /// @return Returns a const iterator to the first TextLine being stored by this TextLayer.
            virtual ConstTextLineIterator BeginTextLine() const;
            /// @brief Gets a const iterator to one passed the last TextLine.
            /// @return Returns a const iterator to one passed the last TextLine being stored by this TextLayer.
            virtual ConstTextLineIterator EndTextLine() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Character Methods

            /// @brief Gets a Character by index.
            /// @note The index is counted from the left to the right, regardless of text order.
            /// @param Index The index of the Character to retrieve.
            /// @return Returns a pointer to the requested Character.
            virtual Character* GetCharacterAtIndex(const Integer Index) const;
            /// @brief Gets a Character by offset position.
            /// @warning Depending on what has been updated there is a chance this may not get the proper character
            /// for the current configuration until after the next frame is drawn.  In most cases this will be safe,
            /// but may not always be.
            /// @param Offset The position relative to the top-left corner of this layer.
            /// @return Returns a pointer to the character under the provided position, or NULL if the position is invalid.
            virtual Character* GetCharacterAtOffset(const Vector2& Offset) const;
            /// @brief Gets an iterator to the character at the specified index.
            /// @param Index The index of the Character to retrieve.
            /// @return Returns an iterator to the requested Character.
            virtual CharacterIterator GetCharacterIteratorAtIndex(const Integer Index);
            /// @brief Gets a const iterator to the character at the specified index.
            /// @param Index The index of the Character to retrieve.
            /// @return Returns a const iterator to the requested Character.
            virtual ConstCharacterIterator GetCharacterIteratorAtIndex(const Integer Index) const;
            /// @brief Gets the number of characters being rendered by this TextLayer.
            /// @return Returns a Whole representing the number of rendered characters being rendered by this TextLayer.
            virtual Whole GetNumCharacters() const;

            /// @brief Creates a character from a Glyph ID and inserts it into the layer at the specified index.
            /// @param Index The index at which the character should be inserted.
            /// @param GlyphID The ID of the Glyph to be inserted.
            /// @return Returns the number of render characters successfully added to this layer.  Negative number means the operation removed render characters.
            virtual Integer InsertCharacterAtIndex(const Integer Index, const UInt32 GlyphID);
            /// @brief Creates a series of characters from a UTF-8 encoded string to be inserted into this layer.
            /// @param Index The index at which the characters will be inserted.
            /// @param Characters An array of Char8's encoded in UTF-8 to be inserted.
            /// @param BufSize The size of the array of Char8's passed in.
            /// @return Returns the number of render characters successfully added to this layer.  Negative number means the operation removed render characters.
            virtual Integer InsertCharactersAtIndex(const Integer Index, const Char8* Characters, const UInt32 BufSize);
            /// @brief Creates a series of characters from a UTF-32 encoded string to be inserted into this layer.
            /// @param Index The index at which the characters will be inserted.
            /// @param Characters An array of UInt32's encoded in UTF-32 to be inserted.
            /// @param BufSize The size of the array of Char8's passed in.
            /// @return Returns the number of render characters successfully added to this layer.  Negative number means the operation removed render characters.
            virtual Integer InsertCharactersAtIndex(const Integer Index, const UInt32* Characters, const UInt32 BufSize);
            /// @brief Removes a character from the layer at the specified index.
            /// @param Index The index of the character to be removed.
            /// @return Returns the number of render characters successfully removed from this layer.  Negative number means the operation added render characters.
            virtual Integer RemoveCharacterAtIndex(const Integer Index);
            /// @brief Removes a length of characters from this layer at the specified index.
            /// @param Index The index of the start of the range of characters to be removed.
            /// @param Length The number of characters being removed.
            /// @return Returns the number of render characters successfully removed from this layer.  Negative number means the operation added render characters.
            virtual Integer RemoveCharactersAtIndex(const Integer Index, const UInt32 Length);
            /// @brief Removes a range of characters from the text in this layer.
            /// @param First The first character in the range to be removed.
            /// @param Last The last character in the range to be removed.
            /// @return Returns the number of render characters successfully removed from this layer.  Negative number means the operation added render characters.
            virtual Integer RemoveCharacterRange(const Integer First, const Integer Last);
            /// @brief Destroy's all characters in this TextLayer
            /// @return Returns the number of render characters successfully removed from this layer.  Negative number means the operation added render characters.  This should never happen.
            virtual Integer DestroyAllCharacters();

            /// @brief Gets an iterator to the first Character.
            /// @return Returns an iterator to the first Character being stored by this TextLine.
            CharacterIterator BeginCharacter();
            /// @brief Gets an iterator to one passed the last Character.
            /// @return Returns an iterator to one passed the last Character being stored by this TextLine.
            CharacterIterator EndCharacter();
            /// @brief Gets a const iterator to the first Character.
            /// @return Returns a const iterator to the first Character being stored by this TextLine.
            ConstCharacterIterator BeginCharacter() const;
            /// @brief Gets an iterator to one passed the last Character.
            /// @return Returns an iterator to one passed the last Character being stored by this TextLine.
            ConstCharacterIterator EndCharacter() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc SimpleRenderer::ProtoSerialize(XML::Node& ParentNode) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @copydoc SimpleRenderer::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the TextCursor of this class to an XML::Node ready for seriailization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeCursor(XML::Node& SelfRoot) const;
            /// @brief Convert the Text of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeText(XML::Node& SelfRoot) const;

            /// @copydoc SimpleRenderer::ProtoDeSerialize(const XML::Node& SelfRoot)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @copydoc SimpleRenderer::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the TextCursor of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeCursor(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the Text of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeText(const XML::Node& SelfRoot);

            /// @copydoc SimpleRenderer::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc SimpleRenderer::GetSerializableName()
            static String GetSerializableName();
        };//TextLayer
    }//UI
}//Mezzanine

#endif
